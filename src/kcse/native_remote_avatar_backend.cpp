#include "kcse/native_remote_avatar_backend.hpp"

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
	}

	native_remote_avatar_backend::native_remote_avatar_backend(
	    native_entity_backend &entities) :
	    m_entities(entities)
	{
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
			const npc::equipment_definition *probe_equipment{};
			for (const auto &candidate :
			     npc::runtime_equipment_catalog().entries())
			{
				CryGUID guid{};
				if (wh::ParseGuid(candidate.definition_id.c_str(), guid)
				    && database->FindClassByGuid(guid))
				{
					probe_equipment = &candidate;
					if (candidate.weapon != npc::weapon_class::none)
						break;
				}
			}
			if (!probe_equipment)
			{
				error =
				    "active probe found no native equipment definition";
				return active_probe_result::failed;
			}

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
		if (!context || !context->m_pActorSystem || !environment
		    || !environment->pEntitySystem
		    || !wh::rpgmodule::C_SoulList::GetInstance())
		{
			m_diagnostic =
			    "native ActorSystem, EntitySystem, or SoulList is unavailable";
			return false;
		}
		std::string error;
		if (!npc::initialize_runtime_catalog(error)
		    || !npc::initialize_runtime_equipment_catalog(error))
		{
			m_diagnostic = std::move(error);
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
		if (!available() || !player.has_transform || !player.has_avatar
		    || !npc::runtime_catalog().contains(
		        player.avatar.archetype_id()))
		{
			m_diagnostic =
			    "remote avatar references an unknown native Soul";
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
		m_entities.begin_player_spawn();
		auto *actor_interface = context->m_pActorSystem->CreateActor(
		    0,
		    name.c_str(),
		    "NPC",
		    &position,
		    &rotation,
		    &scale,
		    0);
		m_entities.end_player_spawn();
		auto *entity =
		    actor_interface ? actor_interface->GetEntity() : nullptr;
		if (!actor_interface || !entity)
		{
			m_diagnostic = "IActorSystem::CreateActor(NPC) failed";
			return std::nullopt;
		}

		const auto id = entity->GetId();
		m_entities.register_player_entity(id);
		auto flags = entity->GetFlags();
		flags &= ~(entity_flag_calc_physics | entity_flag_has_ai
		    | entity_flag_trigger_areas);
		flags |= entity_flag_no_save | entity_flag_clientside_state
		    | entity_flag_no_proximity;
		entity->SetFlags(flags);
		entity->SetAIObjectID(0);
		reinterpret_cast<CEntity *>(entity)->EnablePhysics(false);
		entity->Hide(false);
		entity->Activate(true);

		m_avatars.emplace(
		    handle,
		    entry{
		        .entity_id = id,
		        .epoch = m_epoch,
		        .shared_soul_guid = player.avatar.archetype_id()});
		return handle;
	}

	remote_avatar_backend_status native_remote_avatar_backend::status(
	    remote_avatar_handle avatar) const
	{
		auto *value = const_cast<entry *>(find(avatar));
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
		if (!entity || !actor)
			return {
			    remote_avatar_state::failed,
			    "remote avatar entity was destroyed externally"};
		auto *soul = actor->m_pSoul;
		if (!soul)
			return {
			    remote_avatar_state::pending,
			    "waiting for native Soul"};
		if (!value->shared_soul_applied)
		{
			CryGUID guid{};
			auto *souls = wh::rpgmodule::C_SoulList::GetInstance();
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
		}
		if (!soul->m_inventorySoul.GetInventory()
		    || !soul->m_inventorySoul.GetEquipmentManager())
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
			return false;
		std::string error;
		if (!m_entities.write_transform(entity, player.transform, error)
		    || !drive_motion(*value, player, error))
		{
			value->failed = true;
			value->failure = std::move(error);
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
		if (!actor || !soul || !inventory || !database)
		{
			error = "remote Human/Soul/Inventory readiness was lost";
			return false;
		}

		struct desired_item
		{
			const protocol::AvatarEquipment *wire{};
			const npc::equipment_definition *definition{};
			CryGUID guid{};
		};
		std::vector<desired_item> desired;
		desired.reserve(appearance.equipment_size());
		for (const auto &wire : appearance.equipment())
		{
			CryGUID guid{};
			const auto *definition =
			    npc::runtime_equipment_catalog().find(
			        wire.definition_id());
			if (!definition
			    || definition->equipped_slot != wire.equipped_slot()
			    || !wh::ParseGuid(wire.definition_id().c_str(), guid)
			    || !database->FindClassByGuid(guid))
			{
				error =
				    "remote equipment definition/slot is invalid: "
				    + wire.definition_id();
				return false;
			}
			desired.push_back({&wire, definition, guid});
		}
		std::ranges::sort(
		    desired,
		    [](const desired_item &left, const desired_item &right)
		    {
			    return left.definition->layer
			        < right.definition->layer;
		    });
		if (!remove_created_items(avatar, error))
			return false;

		for (const auto &item : desired)
		{
			auto *created =
			    inventory->CreateItem(item.guid, 1.0F, 1);
			if (!created)
			{
				error = "remote native item creation failed";
				return false;
			}
			const auto instance = make_instance_guid();
			created->SetInstanceGuid(instance);
			const auto instance_text = wh::FormatGuid(instance);
			avatar.item_instances.push_back(instance_text);
			soul->m_inventorySoul.EquipItem(created, true);
			if ((created->m_flags & item_equipped) == 0)
			{
				error =
				    "remote native EquipItem did not set equipped state";
				return false;
			}
		}

		auto *human =
		    reinterpret_cast<wh::entitymodule::C_Human *>(actor);
		const bool should_draw = appearance.weapon_drawn()
		    && appearance.weapon_class()
		        != protocol::AVATAR_WEAPON_CLASS_NONE;
		if (human->IsWeaponDrawn() != should_draw
		    && !human->SetWeaponDrawn(should_draw))
		{
			error = should_draw
			    ? "native DrawWeapon failed"
			    : "native HolsterWeapon failed";
			return false;
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
			return false;
		}
		return true;
	}
}
