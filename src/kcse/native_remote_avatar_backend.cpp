#include "kcse/native_remote_avatar_backend.hpp"
#include "kcse/native_equipment.hpp"
#include "kcse/join_trace.hpp"

#include "npc/equipment_catalog.hpp"

#include <entitymodule/C_Actor.h>
#include <entitymodule/C_Human.h>
#include <entitymodule/C_Inventory.h>
#include <entitymodule/C_Item.h>
#include <entitymodule/C_ItemDatabase.h>
#include <framework/GuidUtils.h>
#include <game/S_GameContext.h>
#include <rpgmodule/C_Soul.h>
#include <rpgmodule/C_SoulList.h>
#include <crysystem/CEntity.h>
#include <crysystem/SSystemGlobalEnvironment.h>
#include <Offsets/vtables/IEntity.h>
#include <Offsets/vtables/IEntitySystem.h>
#include <Offsets/vtables/IActor.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <format>
#include <limits>
#include <random>
#include <ranges>

namespace kcd2mp::kcse
{
	namespace
	{
		constexpr std::uint32_t item_equipped = 1U;
		constexpr std::uint32_t entity_flag_calc_physics = 1U << 7;
		constexpr std::uint32_t entity_flag_has_ai = 1U << 13;
		constexpr std::uint32_t entity_flag_trigger_areas = 1U << 14;
		constexpr std::uint32_t entity_flag_no_save = 1U << 15;
		constexpr std::uint32_t entity_flag_clientside_state = 1U << 17;
		constexpr std::uint32_t entity_flag_no_proximity = 1U << 19;
		constexpr std::string_view probe_equipment_definition_id =
		    "c164f346-0463-4116-b790-094b11274e5e";

		CryGUID make_instance_guid()
		{
			thread_local std::mt19937_64 generator{
			    std::random_device{}()
			    ^ static_cast<std::uint64_t>(
			        std::chrono::high_resolution_clock::now()
			            .time_since_epoch()
			            .count())};
			CryGUID result{};
			static_assert(sizeof(result) == 16);
			auto *parts = reinterpret_cast<std::uint64_t *>(&result);
			parts[0] = generator();
			parts[1] = generator();
			auto *bytes = reinterpret_cast<std::uint8_t *>(&result);
			bytes[6] = static_cast<std::uint8_t>(
			    (bytes[6] & 0x0FU) | 0x40U);
			bytes[8] = static_cast<std::uint8_t>(
			    (bytes[8] & 0x3FU) | 0x80U);
			return result;
		}

		Quat native_rotation(const protocol::Quaternion &rotation)
		{
			return Quat(
			    rotation.w(),
			    rotation.x(),
			    rotation.y(),
			    rotation.z());
		}

		Vec3 native_position(const protocol::Vec3 &position)
		{
			return Vec3(position.x(), position.y(), position.z());
		}

		wh::entitymodule::C_Actor *resolve_actor(
		    std::uint32_t entity_id)
		{
			auto *context = wh::game::S_GameContext::GetInstance();
			return context ? context->GetActorById(entity_id) : nullptr;
		}

		Offsets::IEntity *resolve_entity(std::uint32_t entity_id)
		{
			auto *environment = SSystemGlobalEnvironment::GetInstance();
			return environment && environment->pEntitySystem
			    ? environment->pEntitySystem->GetEntity(entity_id)
			    : nullptr;
		}

		wh::entitymodule::C_Item *find_item(
		    wh::entitymodule::C_Inventory &inventory,
		    std::string_view instance)
		{
			const auto found = std::ranges::find_if(
			    inventory.m_items,
			    [&](const auto *item)
			    {
				    return item
				        && wh::FormatGuid(item->m_instanceGuid) == instance;
			    });
			return found == inventory.m_items.end() ? nullptr : *found;
		}

		protocol::AvatarWeaponClass protocol_weapon(
		    npc::weapon_class value)
		{
			switch (value)
			{
			case npc::weapon_class::one_handed:
				return protocol::AVATAR_WEAPON_CLASS_ONE_HANDED;
			case npc::weapon_class::two_handed:
				return protocol::AVATAR_WEAPON_CLASS_TWO_HANDED;
			case npc::weapon_class::polearm:
				return protocol::AVATAR_WEAPON_CLASS_POLEARM;
			case npc::weapon_class::bow:
				return protocol::AVATAR_WEAPON_CLASS_BOW;
			case npc::weapon_class::crossbow:
				return protocol::AVATAR_WEAPON_CLASS_CROSSBOW;
			case npc::weapon_class::none:
				return protocol::AVATAR_WEAPON_CLASS_NONE;
			}
			return protocol::AVATAR_WEAPON_CLASS_NONE;
		}

		Offsets::IActor *guarded_create_actor(
		    Offsets::IActorSystem *actor_system,
		    const char *name,
		    const Vec3 *position,
		    const Quat *rotation,
		    const Vec3 *scale) noexcept
		{
#ifdef _WIN32
			__try
			{
				return actor_system->CreateActor(
				    0,
				    name,
				    "NPC",
				    position,
				    rotation,
				    scale,
				    0);
			}
			__except(KCD2MP_JOIN_SEH_FILTER(
			    "join.remote-spawn.CreateActor.seh"))
			{
				return nullptr;
			}
#else
			return actor_system->CreateActor(
			    0,
			    name,
			    "NPC",
			    position,
			    rotation,
			    scale,
			    0);
#endif
		}
	}

	native_remote_avatar_backend::native_remote_avatar_backend(
	    native_entity_backend &entities) :
	    m_entities(entities)
	{
	}

	void native_remote_avatar_backend::advance_frame() noexcept
	{
		++m_frame_sequence;
	}

	void native_remote_avatar_backend::set_epoch(std::uint64_t epoch)
	{
		if (m_epoch == epoch)
			return;
		clear();
		m_epoch = epoch;
	}

	void native_remote_avatar_backend::clear()
	{
		std::vector<remote_avatar_handle> handles;
		handles.reserve(m_avatars.size());
		for (const auto &[handle, value] : m_avatars)
		{
			(void)value;
			handles.push_back(handle);
		}
		for (const auto handle : handles)
			remove(handle);
		m_avatars.clear();
		m_probe_avatar.reset();
		m_probe_polls = 0;
	}

	void native_remote_avatar_backend::reset_active_probe()
	{
		if (m_probe_avatar)
			remove(*m_probe_avatar);
		m_probe_avatar.reset();
		m_probe_snapshot = {};
		m_probe_polls = 0;
	}

	native_remote_avatar_backend::active_probe_result
	native_remote_avatar_backend::poll_active_probe(
	    const protocol::TransformState &origin,
	    std::string &error)
	{
		if (!m_probe_avatar)
		{
			if (!available())
			{
				error = diagnostic();
				return active_probe_result::failed;
			}
			auto *database = wh::entitymodule::C_ItemDatabase::GetInstance();
			if (!database)
			{
				error = "active probe has no native item database";
				return active_probe_result::failed;
			}
			auto &catalog = npc::runtime_equipment_catalog();
			const npc::equipment_definition *probe_equipment =
			    catalog.find(probe_equipment_definition_id);
			auto is_native_item = [database](
			                          const npc::equipment_definition *candidate)
			{
				if (!candidate)
					return false;
				CryGUID guid{};
				return wh::ParseGuid(
				           candidate->definition_id.c_str(),
				           guid)
				    && database->FindClassByGuid(guid);
			};
			if (!is_native_item(probe_equipment))
				probe_equipment = nullptr;
			if (!probe_equipment)
			{
				for (const auto &candidate : catalog.entries())
				{
					if (candidate.equipped_slot == "PrimaryMainHand"
					    && candidate.weapon
					        == npc::weapon_class::one_handed
					    && is_native_item(&candidate))
					{
						probe_equipment = &candidate;
						break;
					}
				}
			}
			if (!probe_equipment)
			{
				error =
				    "active probe found no native equipment definition";
				return active_probe_result::failed;
			}
			KCD2MP_JOIN_TRACE(
			    "join.native-probe.equipment-selected",
			    std::format(
			        "definition_id={} equipped_slot={} weapon_class={}",
			        probe_equipment->definition_id,
			        probe_equipment->equipped_slot,
			        static_cast<int>(probe_equipment->weapon)));

			m_probe_snapshot = {};
			m_probe_snapshot.id =
			    std::numeric_limits<std::uint64_t>::max();
			m_probe_snapshot.display_name = "KCD2MP native ABI probe";
			m_probe_snapshot.connected = true;
			m_probe_snapshot.has_transform = true;
			m_probe_snapshot.transform = origin;
			m_probe_snapshot.movement_mode =
			    protocol::MOVEMENT_MODE_IDLE;
			m_probe_snapshot.has_avatar = true;
			m_probe_snapshot.avatar.set_archetype_id(
			    npc::default_soul_id);
			m_probe_snapshot.avatar.set_revision(1);
			m_probe_snapshot.avatar.set_stance(
			    protocol::AVATAR_STANCE_RELAXED);
			m_probe_snapshot.avatar.set_weapon_class(
			    protocol_weapon(probe_equipment->weapon));
			m_probe_snapshot.avatar.set_weapon_drawn(false);
			auto *equipment =
			    m_probe_snapshot.avatar.add_equipment();
			equipment->set_definition_id(
			    probe_equipment->definition_id);
			equipment->set_equipped_slot(
			    probe_equipment->equipped_slot);

			m_probe_avatar = spawn(m_probe_snapshot);
			if (!m_probe_avatar)
			{
				error = diagnostic();
				return active_probe_result::failed;
			}
			if (auto *value = find(*m_probe_avatar))
			{
				if (auto *entity = resolve_entity(value->entity_id))
				{
					entity->Activate(false);
					entity->Hide(true);
				}
			}
			return active_probe_result::pending;
		}

		if (++m_probe_polls > 600)
		{
			error = "active native Avatar probe timed out";
			reset_active_probe();
			return active_probe_result::failed;
		}
		const auto lifecycle = status(*m_probe_avatar);
		if (lifecycle.state == remote_avatar_state::pending)
			return active_probe_result::pending;
		if (lifecycle.state == remote_avatar_state::failed)
		{
			error = lifecycle.diagnostic;
			reset_active_probe();
			return active_probe_result::failed;
		}
		if (!update(*m_probe_avatar, m_probe_snapshot, true))
		{
			error = diagnostic();
			if (const auto *value = find(*m_probe_avatar);
			    value && !value->failure.empty())
				error = value->failure;
			reset_active_probe();
			return active_probe_result::failed;
		}

		auto *value = find(*m_probe_avatar);
		auto *actor = value ? resolve_actor(value->entity_id) : nullptr;
		auto *soul = actor ? actor->m_pSoul : nullptr;
		auto *inventory =
		    soul ? soul->m_inventorySoul.GetInventory() : nullptr;
		if (!value || !inventory || value->item_instances.size() != 1)
		{
			error =
			    "active native probe did not create exactly one item";
			reset_active_probe();
			return active_probe_result::failed;
		}
		auto *item = find_item(*inventory, value->item_instances.front());
		if (!item || (item->m_flags & item_equipped) == 0)
		{
			error = "active native probe item was not equipped";
			reset_active_probe();
			return active_probe_result::failed;
		}

		const auto entity_id = value->entity_id;
		const auto handle = *m_probe_avatar;
		m_probe_avatar.reset();
		remove(handle);
		if (resolve_entity(entity_id))
		{
			error =
			    "active native probe entity survived forced removal";
			return active_probe_result::failed;
		}
		m_probe_polls = 0;
		error.clear();
		return active_probe_result::succeeded;
	}

	bool native_remote_avatar_backend::available() const
	{
		auto *context = wh::game::S_GameContext::GetInstance();
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		KCD2MP_JOIN_TRACE(
		    "join.remote-backend.precheck",
		    std::format(
		        "game_context={} actor_system={} environment={} "
		        "entity_system={} soul_list={}",
		        static_cast<void *>(context),
		        context
		            ? static_cast<void *>(context->m_pActorSystem)
		            : nullptr,
		        static_cast<void *>(environment),
		        environment
		            ? static_cast<void *>(environment->pEntitySystem)
		            : nullptr,
		        static_cast<void *>(
		            wh::rpgmodule::C_SoulList::GetInstance())));
		if (!context || !context->m_pActorSystem || !environment
		    || !environment->pEntitySystem
		    || !wh::rpgmodule::C_SoulList::GetInstance())
		{
			m_diagnostic =
			    "native ActorSystem, EntitySystem, or SoulList is unavailable";
			KCD2MP_JOIN_TRACE(
			    "join.remote-backend.unavailable",
			    m_diagnostic);
			return false;
		}
		std::string error;
		if (!npc::initialize_runtime_catalog(error)
		    || !npc::initialize_runtime_equipment_catalog(error))
		{
			m_diagnostic = std::move(error);
			KCD2MP_JOIN_TRACE(
			    "join.remote-backend.catalog-failed",
			    m_diagnostic);
			return false;
		}
		m_diagnostic.clear();
		return true;
	}

	std::string native_remote_avatar_backend::diagnostic() const
	{
		return m_diagnostic.empty()
		    ? "native remote-avatar backend is unavailable"
		    : m_diagnostic;
	}

	std::optional<remote_avatar_handle>
	native_remote_avatar_backend::spawn(
	    const remote_avatar_snapshot &player)
	{
		const auto existing = std::ranges::count_if(
		    m_avatars,
		    [&](const auto &pair)
		    {
			    return pair.second.player == player.id;
		    });
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.request",
		    std::format(
		        "player_id={} display_name=\"{}\" existing_puppets={} "
		        "has_transform={} has_avatar={} soul=\"{}\"",
		        player.id,
		        player.display_name,
		        existing,
		        player.has_transform,
		        player.has_avatar,
		        player.avatar.archetype_id()));
		if (existing != 0)
		{
			KCD2MP_JOIN_TRACE(
			    "join.remote-spawn.duplicate-detected",
			    std::format(
			        "player_id={} existing_puppets={} "
			        "spawn_is_not_a_silent_overwrite",
			        player.id,
			        existing));
		}
		if (!available())
		{
			KCD2MP_JOIN_TRACE(
			    "join.remote-spawn.rejected",
			    m_diagnostic);
			return std::nullopt;
		}
		if (!player.has_transform)
		{
			m_diagnostic = "remote avatar has no transform";
			KCD2MP_JOIN_TRACE(
			    "join.remote-spawn.rejected",
			    m_diagnostic);
			return std::nullopt;
		}
		if (!player.has_avatar)
		{
			m_diagnostic = "remote avatar has no avatar descriptor";
			KCD2MP_JOIN_TRACE(
			    "join.remote-spawn.rejected",
			    m_diagnostic);
			return std::nullopt;
		}
		if (!npc::runtime_catalog().contains(
		        player.avatar.archetype_id()))
		{
			m_diagnostic =
			    "remote avatar references an unknown native Soul";
			KCD2MP_JOIN_TRACE(
			    "join.remote-spawn.rejected",
			    std::format(
			        "soul=\"{}\" error=\"{}\"",
			        player.avatar.archetype_id(),
			        m_diagnostic));
			return std::nullopt;
		}
		auto *context = wh::game::S_GameContext::GetInstance();
		const auto position = native_position(player.transform.position());
		const auto rotation = native_rotation(player.transform.rotation());
		const Vec3 scale(1.0F, 1.0F, 1.0F);
		const auto handle = m_next_handle++;
		const auto name = std::format(
		    "KCD2MP_Remote_{}_{}_{}",
		    m_epoch,
		    player.id,
		    handle);
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.engine-call.begin",
		    std::format(
		        "api=IActorSystem::CreateActor channel=0 name=\"{}\" "
		        "class=\"NPC\" template=<not-used-by-CreateActor> "
		        "soul=\"{}\" position=({:.6f},{:.6f},{:.6f}) "
		        "rotation=({:.6f},{:.6f},{:.6f},{:.6f}) "
		        "scale=(1,1,1) requested_entity_id=0 "
		        "actor_system={} entity_system={}",
		        name,
		        player.avatar.archetype_id(),
		        position.x,
		        position.y,
		        position.z,
		        rotation.v.x,
		        rotation.v.y,
		        rotation.v.z,
		        rotation.w,
		        static_cast<void *>(context->m_pActorSystem),
		        SSystemGlobalEnvironment::GetInstance()
		                ? static_cast<void *>(
		                      SSystemGlobalEnvironment::GetInstance()
		                          ->pEntitySystem)
		                : nullptr));
		m_entities.begin_player_spawn();
		auto *actor_interface = guarded_create_actor(
		    context->m_pActorSystem,
		    name.c_str(),
		    &position,
		    &rotation,
		    &scale);
		m_entities.end_player_spawn();
		KCD2MP_JOIN_TRACE(
		    actor_interface
		        ? "join.remote-spawn.engine-call.returned"
		        : "join.remote-spawn.engine-call.nil",
		    std::format(
		        "api=IActorSystem::CreateActor actor={}",
		        static_cast<void *>(actor_interface)));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.GetEntity.begin",
		    std::format(
		        "actor={}",
		        static_cast<void *>(actor_interface)));
		auto *entity =
		    actor_interface ? actor_interface->GetEntity() : nullptr;
		KCD2MP_JOIN_TRACE(
		    entity ? "join.remote-spawn.entity.resolved"
		           : "join.remote-spawn.entity.nil",
		    std::format(
		        "actor={} entity={}",
		        static_cast<void *>(actor_interface),
		        static_cast<void *>(entity)));
		if (!actor_interface || !entity)
		{
			m_diagnostic = "IActorSystem::CreateActor(NPC) failed";
			KCD2MP_JOIN_TRACE(
			    "join.remote-spawn.failed",
			    m_diagnostic);
			return std::nullopt;
		}

		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.GetId.begin",
		    std::format("entity={}", static_cast<void *>(entity)));
		const auto id = entity->GetId();
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.GetId.returned",
		    std::format("entity_id={}", id));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.entity.configure.begin",
		    std::format(
		        "player_id={} handle={} entity_id={} entity={}",
		        player.id,
		        handle,
		        id,
		        static_cast<void *>(entity)));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.register-player-entity.begin",
		    std::format("entity_id={}", id));
		m_entities.register_player_entity(id);
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.register-player-entity.returned",
		    std::format("entity_id={}", id));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.GetFlags.begin",
		    std::format("entity_id={}", id));
		auto flags = entity->GetFlags();
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.GetFlags.returned",
		    std::format("entity_id={} flags=0x{:08X}", id, flags));
		flags &= ~(entity_flag_calc_physics | entity_flag_has_ai
		    | entity_flag_trigger_areas);
		flags |= entity_flag_no_save | entity_flag_clientside_state
		    | entity_flag_no_proximity;
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.SetFlags.begin",
		    std::format("entity_id={} flags=0x{:08X}", id, flags));
		entity->SetFlags(flags);
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.SetFlags.returned",
		    std::format("entity_id={}", id));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.SetAIObjectID.begin",
		    std::format("entity_id={} ai_object_id=0", id));
		entity->SetAIObjectID(0);
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.SetAIObjectID.returned",
		    std::format("entity_id={}", id));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.EnablePhysics.begin",
		    std::format(
		        "entity_id={} entity={} enabled=false "
		        "api=fork:CEntity::EnablePhysics",
		        id,
		        static_cast<void *>(entity)));
		const auto physics_result =
		    reinterpret_cast<CEntity *>(entity)->EnablePhysics(false);
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.EnablePhysics.returned",
		    std::format(
		        "entity_id={} result={}",
		        id,
		        physics_result));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.Hide.begin",
		    std::format("entity_id={} hidden=false", id));
		entity->Hide(false);
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.Hide.returned",
		    std::format("entity_id={}", id));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.Activate.begin",
		    std::format("entity_id={} active=true", id));
		entity->Activate(true);
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.Activate.returned",
		    std::format("entity_id={}", id));
		KCD2MP_JOIN_TRACE(
		    "join.remote-spawn.entity.configure.complete",
		    std::format(
		        "entity_id={} flags=0x{:08X} ai_object_id=0 "
		        "physics=false hidden=false active=true",
		        id,
		        flags));

		const auto [iterator, inserted] = m_avatars.emplace(
		    handle,
		    entry{
		        .player = player.id,
		        .entity_id = id,
		        .epoch = m_epoch,
		        .shared_soul_guid = player.avatar.archetype_id()});
		(void)iterator;
		KCD2MP_JOIN_TRACE(
		    inserted ? "join.remote-spawn.success"
		             : "join.remote-spawn.handle-collision",
		    std::format(
		        "player_id={} handle={} entity_id={} actor={} entity={}",
		        player.id,
		        handle,
		        id,
		        static_cast<void *>(actor_interface),
		        static_cast<void *>(entity)));
		if (!inserted)
		{
			m_diagnostic = "remote avatar handle collision";
			m_entities.unregister_player_entity(id);
			auto *environment = SSystemGlobalEnvironment::GetInstance();
			if (environment && environment->pEntitySystem)
				environment->pEntitySystem->RemoveEntity(id, true);
			return std::nullopt;
		}
		return handle;
	}

	remote_avatar_backend_status native_remote_avatar_backend::status(
	    remote_avatar_handle avatar) const
	{
		auto *value = const_cast<entry *>(find(avatar));
		KCD2MP_JOIN_TRACE(
		    "join.remote-status.begin",
		    std::format(
		        "handle={} entry={} epoch={} current_epoch={}",
		        avatar,
		        static_cast<void *>(value),
		        value ? value->epoch : 0,
		        m_epoch));
		if (!value)
			return {
			    remote_avatar_state::failed,
			    "remote avatar handle is stale"};
		if (value->failed)
			return {remote_avatar_state::failed, value->failure};
		if (value->epoch != m_epoch)
			return {
			    remote_avatar_state::failed,
			    "remote avatar belongs to a stale runtime epoch"};

		auto *entity = resolve_entity(value->entity_id);
		auto *actor = resolve_actor(value->entity_id);
		KCD2MP_JOIN_TRACE(
		    "join.remote-status.pointer-state",
		    std::format(
		        "player_id={} entity_id={} entity={} actor={}",
		        value->player,
		        value->entity_id,
		        static_cast<void *>(entity),
		        static_cast<void *>(actor)));
		if (!entity || !actor)
			return {
			    remote_avatar_state::failed,
			    "remote avatar entity was destroyed externally"};
		auto *soul = actor->m_pSoul;
		KCD2MP_JOIN_TRACE(
		    soul ? "join.remote-status.soul.ready"
		         : "join.remote-status.soul.pending",
		    std::format(
		        "player_id={} entity_id={} soul={}",
		        value->player,
		        value->entity_id,
		        static_cast<void *>(soul)));
		if (!soul)
			return {
			    remote_avatar_state::pending,
			    "waiting for native Soul"};
		if (!value->shared_soul_applied)
		{
			CryGUID guid{};
			auto *souls = wh::rpgmodule::C_SoulList::GetInstance();
			KCD2MP_JOIN_TRACE(
			    "join.remote-status.ApplySharedSoul.begin",
			    std::format(
			        "player_id={} entity_id={} soul={} soul_list={} "
			        "shared_soul_guid=\"{}\" api=fork:C_SoulList::ApplySharedSoul",
			        value->player,
			        value->entity_id,
			        static_cast<void *>(soul),
			        static_cast<void *>(souls),
			        value->shared_soul_guid));
			if (!souls
			    || !wh::ParseGuid(
			        value->shared_soul_guid.c_str(),
			        guid)
			    || !souls->ApplySharedSoul(*soul, guid))
			{
				value->failed = true;
				value->failure =
				    "native shared-Soul materialization failed";
				return {
				    remote_avatar_state::failed,
				    value->failure};
			}
			value->shared_soul_applied = true;
			value->shared_soul_applied_frame = m_frame_sequence;
			value->shared_soul_applied_at =
			    std::chrono::steady_clock::now();
			KCD2MP_JOIN_TRACE(
			    "join.remote-status.ApplySharedSoul.returned",
			    std::format(
			        "player_id={} entity_id={} result=true",
			        value->player,
			        value->entity_id));
			return {
			    remote_avatar_state::pending,
			    "waiting for native shared-Soul stabilization"};
		}
		const auto settled = evaluate_remote_soul_settle(
		    m_frame_sequence,
		    value->shared_soul_applied_frame,
		    std::chrono::steady_clock::now(),
		    value->shared_soul_applied_at);
		if (!settled.ready)
		{
			KCD2MP_JOIN_TRACE(
			    "join.remote-status.soul-settling",
			    std::format(
			        "player_id={} entity_id={} elapsed_frames={} "
			        "elapsed_ms={} required_frames={} required_ms={}",
			        value->player,
			        value->entity_id,
			        settled.elapsed_frames,
			        settled.elapsed_time.count(),
			        remote_soul_settle_frames,
			        remote_soul_settle_time.count()));
			return {
			    remote_avatar_state::pending,
			    "waiting for native shared-Soul stabilization"};
		}
		auto *inventory = soul->m_inventorySoul.GetInventory();
		auto *equipment =
		    soul->m_inventorySoul.GetEquipmentManager();
		KCD2MP_JOIN_TRACE(
		    "join.remote-status.inventory-state",
		    std::format(
		        "player_id={} entity_id={} inventory={} equipment_manager={}",
		        value->player,
		        value->entity_id,
		        static_cast<void *>(inventory),
		        static_cast<void *>(equipment)));
		if (!inventory || !equipment)
		{
			return {
			    remote_avatar_state::pending,
			    "waiting for native Human inventory/equipment"};
		}
		return {remote_avatar_state::ready, {}};
	}

	bool native_remote_avatar_backend::update(
	    remote_avatar_handle avatar,
	    const remote_avatar_snapshot &player,
	    bool appearance_changed)
	{
		auto *value = find(avatar);
		auto *entity =
		    value && value->epoch == m_epoch
		    ? resolve_entity(value->entity_id)
		    : nullptr;
		if (!value || !entity || value->failed)
		{
			KCD2MP_JOIN_TRACE(
			    "join.remote-update.rejected",
			    std::format(
			        "handle={} entry={} entity={} failed={}",
			        avatar,
			        static_cast<void *>(value),
			        static_cast<void *>(entity),
			        value ? value->failed : false));
			return false;
		}
		std::string error;
		if (!value->first_transform_logged)
		{
			value->first_transform_logged = true;
			KCD2MP_JOIN_TRACE(
			    "join.remote-update.first-transform",
			    std::format(
			        "player_id={} handle={} entity_id={} "
			        "api=IEntity::SetWorldTM position=({:.6f},{:.6f},{:.6f})",
			        value->player,
			        avatar,
			        value->entity_id,
			        player.transform.position().x(),
			        player.transform.position().y(),
			        player.transform.position().z()));
		}
		if (!m_entities.write_transform(entity, player.transform, error)
		    || !drive_motion(*value, player, error))
		{
			value->failed = true;
			value->failure = std::move(error);
			KCD2MP_JOIN_TRACE(
			    "join.remote-update.failed",
			    std::format(
			        "player_id={} handle={} entity_id={} error=\"{}\"",
			        value->player,
			        avatar,
			        value->entity_id,
			        value->failure));
			return false;
		}

		const auto lifecycle = status(avatar);
		if (lifecycle.state == remote_avatar_state::failed)
			return false;
		if (lifecycle.state == remote_avatar_state::ready
		    && (appearance_changed || !value->appearance_applied))
		{
			const auto old = value->appearance;
			const bool had_old = value->appearance_applied;
			if (!apply_appearance(*value, player.avatar, error))
			{
				std::string rollback_error;
				const bool restored = !had_old
				    ? remove_created_items(*value, rollback_error)
				    : apply_appearance(
				        *value,
				        old,
				        rollback_error);
				value->failed = true;
				value->failure =
				    "remote equipment transaction failed: " + error;
				if (!restored)
					value->failure +=
					    "; rollback failed: " + rollback_error;
				return false;
			}
			value->appearance = player.avatar;
			value->appearance_applied = true;
		}
		return true;
	}

	void native_remote_avatar_backend::remove(
	    remote_avatar_handle avatar)
	{
		const auto found = m_avatars.find(avatar);
		if (found == m_avatars.end())
			return;
		const auto id = found->second.entity_id;
		std::string ignored;
		(void)remove_created_items(found->second, ignored);
		m_entities.unregister_player_entity(id);
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		if (environment && environment->pEntitySystem
		    && environment->pEntitySystem->GetEntity(id))
			environment->pEntitySystem->RemoveEntity(id, true);
		m_avatars.erase(found);
	}

	native_remote_avatar_backend::entry *
	native_remote_avatar_backend::find(remote_avatar_handle avatar)
	{
		const auto found = m_avatars.find(avatar);
		return found == m_avatars.end() ? nullptr : &found->second;
	}

	const native_remote_avatar_backend::entry *
	native_remote_avatar_backend::find(remote_avatar_handle avatar) const
	{
		const auto found = m_avatars.find(avatar);
		return found == m_avatars.end() ? nullptr : &found->second;
	}

	bool native_remote_avatar_backend::remove_created_items(
	    entry &avatar,
	    std::string &error)
	{
		auto *actor = resolve_actor(avatar.entity_id);
		auto *soul = actor ? actor->m_pSoul : nullptr;
		auto *inventory =
		    soul ? soul->m_inventorySoul.GetInventory() : nullptr;
		if (!inventory)
		{
			if (avatar.item_instances.empty())
				return true;
			error = "remote inventory disappeared during cleanup";
			return false;
		}
		for (auto iterator = avatar.item_instances.rbegin();
		     iterator != avatar.item_instances.rend();
		     ++iterator)
		{
			auto *item = find_item(*inventory, *iterator);
			if (!item)
				continue;
			if ((item->m_flags & item_equipped) != 0)
			{
				soul->m_inventorySoul.UnequipItem(item, true);
				if ((item->m_flags & item_equipped) != 0)
				{
					error =
					    "remote native UnequipItem left the item equipped";
					return false;
				}
			}
			inventory->RemoveItem(
			    item,
			    2,
			    static_cast<std::uint32_t>(item->m_amount));
			if (find_item(*inventory, *iterator))
			{
				error = "remote item cleanup left an inventory instance";
				return false;
			}
		}
		avatar.item_instances.clear();
		return true;
	}

	bool native_remote_avatar_backend::apply_appearance(
	    entry &avatar,
	    const protocol::AvatarDescriptor &appearance,
	    std::string &error)
	{
		auto *actor = resolve_actor(avatar.entity_id);
		auto *soul = actor ? actor->m_pSoul : nullptr;
		auto *inventory =
		    soul ? soul->m_inventorySoul.GetInventory() : nullptr;
		auto *database = wh::entitymodule::C_ItemDatabase::GetInstance();
		KCD2MP_JOIN_TRACE(
		    "join.remote-appearance.precheck",
		    std::format(
		        "player_id={} entity_id={} actor={} soul={} inventory={} "
		        "item_database={} equipment_count={}",
		        avatar.player,
		        avatar.entity_id,
		        static_cast<void *>(actor),
		        static_cast<void *>(soul),
		        static_cast<void *>(inventory),
		        static_cast<void *>(database),
		        appearance.equipment_size()));
		if (!actor || !soul || !inventory || !database)
		{
			error = "remote Human/Soul/Inventory readiness was lost";
			return false;
		}

		struct desired_item
		{
			const protocol::AvatarEquipment *wire{};
			CryGUID guid{};
			int layer{};
		};
		std::vector<desired_item> desired;
		desired.reserve(appearance.equipment_size());
		for (const auto &wire : appearance.equipment())
		{
			CryGUID guid{};
			if (!wh::ParseGuid(wire.definition_id().c_str(), guid))
			{
				error = "remote equipment definition UUID is invalid: "
				    + wire.definition_id();
				return false;
			}
			if (!database->FindClassByGuid(guid))
			{
				KCD2MP_JOIN_TRACE(
				    "join.remote-appearance.item-skipped",
				    std::format(
				        "player_id={} definition=\"{}\" slot=\"{}\" "
				        "reason=native-definition-unavailable",
				        avatar.player,
				        wire.definition_id(),
				        wire.equipped_slot()));
				continue;
			}
			const auto layer =
			    npc::runtime_equipment_catalog().layer_for_slot(
			        wire.equipped_slot());
			desired.push_back({&wire, guid, layer});
		}
		std::ranges::sort(
		    desired,
		    [](const desired_item &left, const desired_item &right)
		    {
			    return left.layer < right.layer;
			});
		if (!remove_created_items(avatar, error))
			return false;

		bool equipped_supported_weapon = false;
		for (const auto &item : desired)
		{
			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.CreateItem.begin",
			    std::format(
			        "player_id={} entity_id={} definition=\"{}\" "
			        "slot=\"{}\" api=fork:C_InventoryBase::CreateItem",
			        avatar.player,
			        avatar.entity_id,
			        item.wire->definition_id(),
			        item.wire->equipped_slot()));
			auto *created =
			    inventory->CreateItem(item.guid, 1.0F, 1);
			KCD2MP_JOIN_TRACE(
			    created ? "join.remote-appearance.CreateItem.returned"
			            : "join.remote-appearance.CreateItem.nil",
			    std::format(
			        "player_id={} entity_id={} item={}",
			        avatar.player,
			        avatar.entity_id,
			        static_cast<void *>(created)));
			if (!created)
			{
				KCD2MP_JOIN_TRACE(
				    "join.remote-appearance.item-skipped",
				    std::format(
				        "player_id={} definition=\"{}\" slot=\"{}\" "
				        "reason=native-item-creation-failed",
				        avatar.player,
				        item.wire->definition_id(),
				        item.wire->equipped_slot()));
				continue;
			}
			const auto instance = make_instance_guid();
			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.SetInstanceGuid.begin",
			    std::format(
			        "player_id={} entity_id={} item={}",
			        avatar.player,
			        avatar.entity_id,
			        static_cast<void *>(created)));
			created->SetInstanceGuid(instance);
			const auto instance_text = wh::FormatGuid(instance);
			avatar.item_instances.push_back(instance_text);
			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.EquipItem.begin",
			    std::format(
			        "player_id={} entity_id={} item={} instance=\"{}\"",
			        avatar.player,
			        avatar.entity_id,
			        static_cast<void *>(created),
			        instance_text));
			soul->m_inventorySoul.EquipItem(created, true);
			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.EquipItem.returned",
			    std::format(
			        "player_id={} entity_id={} item_flags=0x{:08X}",
			        avatar.player,
			        avatar.entity_id,
			        created->m_flags));
			if ((created->m_flags & item_equipped) == 0)
			{
				KCD2MP_JOIN_TRACE(
				    "join.remote-appearance.item-skipped",
				    std::format(
				        "player_id={} definition=\"{}\" slot=\"{}\" "
				        "reason=native-equip-rejected",
				        avatar.player,
				        item.wire->definition_id(),
				        item.wire->equipped_slot()));
				inventory->RemoveItem(created, 2, 1);
				if (find_item(*inventory, instance_text))
				{
					error =
					    "remote rejected item cleanup left an inventory instance";
					return false;
				}
				avatar.item_instances.pop_back();
				continue;
			}

			auto *equipment =
			    soul->m_inventorySoul.GetEquipmentManager();
			const auto actual_slot = equipment
			    ? native_equipped_slot(*equipment, *created)
			    : std::nullopt;
			if (!actual_slot || *actual_slot != item.wire->equipped_slot())
			{
				KCD2MP_JOIN_TRACE(
				    "join.remote-appearance.item-skipped",
				    std::format(
				        "player_id={} definition=\"{}\" requested_slot=\"{}\" "
				        "actual_slot=\"{}\" reason=native-slot-mismatch",
				        avatar.player,
				        item.wire->definition_id(),
				        item.wire->equipped_slot(),
				        actual_slot.value_or("")));
				soul->m_inventorySoul.UnequipItem(created, true);
				if ((created->m_flags & item_equipped) != 0)
				{
					error =
					    "remote mismatched item could not be unequipped";
					return false;
				}
				inventory->RemoveItem(created, 2, 1);
				if (find_item(*inventory, instance_text))
				{
					error =
					    "remote mismatched item cleanup left an inventory instance";
					return false;
				}
				avatar.item_instances.pop_back();
				continue;
			}
			const auto fixed_weapon = std::ranges::find(
			    native_weapon_equipment_slots,
			    *actual_slot);
			equipped_supported_weapon = equipped_supported_weapon
			    || (fixed_weapon != native_weapon_equipment_slots.end()
			        && *actual_slot != "Torch");
		}

		auto *human =
		    reinterpret_cast<wh::entitymodule::C_Human *>(actor);
		const bool should_draw = appearance.weapon_drawn()
		    && appearance.weapon_class()
		        != protocol::AVATAR_WEAPON_CLASS_NONE
		    && equipped_supported_weapon;
		if (human->IsWeaponDrawn() != should_draw)
		{
			if (!avatar.first_weapon_action_logged)
			{
				avatar.first_weapon_action_logged = true;
				KCD2MP_JOIN_TRACE(
				    "join.remote-animation.first-weapon-action",
				    std::format(
				        "player_id={} entity_id={} requested_drawn={} "
				        "weapon_class={} api=C_Human::SetWeaponDrawn",
				        avatar.player,
				        avatar.entity_id,
				        should_draw,
				        static_cast<int>(appearance.weapon_class())));
			}
			KCD2MP_JOIN_TRACE(
			    "join.remote-animation.weapon-action.begin",
			    std::format(
			        "player_id={} entity_id={} requested_drawn={}",
			        avatar.player,
			        avatar.entity_id,
			        should_draw));
			if (!human->SetWeaponDrawn(should_draw))
			{
				error = should_draw
				    ? "native DrawWeapon failed"
				    : "native HolsterWeapon failed";
				return false;
			}
			KCD2MP_JOIN_TRACE(
			    "join.remote-animation.weapon-action.returned",
			    std::format(
			        "player_id={} entity_id={} result=true",
			        avatar.player,
			        avatar.entity_id));
		}
		return true;
	}

	bool native_remote_avatar_backend::drive_motion(
	    entry &avatar,
	    const remote_avatar_snapshot &player,
	    std::string &error)
	{
		auto *actor = resolve_actor(avatar.entity_id);
		auto *controller =
		    actor ? actor->m_pMovementController : nullptr;
		if (!avatar.first_motion_logged)
		{
			avatar.first_motion_logged = true;
			KCD2MP_JOIN_TRACE(
			    "join.remote-animation.first-locomotion",
			    std::format(
			        "player_id={} entity_id={} actor={} controller={} "
			        "movement_mode={} api=C_Actor::RequestLocomotion",
			        avatar.player,
			        avatar.entity_id,
			        static_cast<void *>(actor),
			        static_cast<void *>(controller),
			        static_cast<int>(player.movement_mode)));
		}
		if (!actor || !controller)
		{
			// Controller construction is part of the asynchronous readiness
			// chain; transform interpolation still applies in the meantime.
			return true;
		}
		float speed{};
		switch (player.movement_mode)
		{
		case protocol::MOVEMENT_MODE_WALK:
			speed = 1.5F;
			break;
		case protocol::MOVEMENT_MODE_RUN:
			speed = 4.5F;
			break;
		case protocol::MOVEMENT_MODE_IDLE:
		default:
			break;
		}
		std::optional<Vec3> move_target;
		if (speed > 0.0F)
		{
			const auto &velocity = player.transform.velocity();
			Vec3 direction(velocity.x(), velocity.y(), velocity.z());
			const auto length = direction.GetLength();
			if (length > 0.001F)
				direction /= length;
			else
			{
				auto *entity = resolve_entity(avatar.entity_id);
				if (entity)
					entity->GetForwardDir(direction);
			}
			move_target =
			    native_position(player.transform.position())
			    + direction * 2.0F;
		}
		if (!actor->RequestLocomotion(
		        move_target ? &*move_target : nullptr,
		        speed))
		{
			error = "native MovementController rejected locomotion request";
			KCD2MP_JOIN_TRACE(
			    "join.remote-animation.locomotion-failed",
			    std::format(
			        "player_id={} entity_id={} speed={} error=\"{}\"",
			        avatar.player,
			        avatar.entity_id,
			        speed,
			        error));
			return false;
		}
		return true;
	}
}
