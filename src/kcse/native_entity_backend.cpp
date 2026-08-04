#include "kcse/native_entity_backend.hpp"
#include "kcse/native_combat_observer.hpp"
#include "kcse/join_trace.hpp"
#include "multiplayer/protocol.hpp"

#include <crysystem/CCryAction.h>
#include <crysystem/SSystemGlobalEnvironment.h>
#include <entitymodule/C_Actor.h>
#include <game/S_GameContext.h>
#include <Offsets/vtables/IEntity.h>
#include <Offsets/vtables/IEntityIt.h>
#include <Offsets/vtables/IEntitySystem.h>
#include <Offsets/vtables/IConsole.h>
#include <Offsets/vtables/ICVar.h>

#include <chrono>
#include <cstddef>
#include <cmath>
#include <format>
#include <string_view>
#include <thread>
#include <unordered_set>
#include <utility>

namespace kcd2mp::kcse
{
	namespace
	{
		constexpr std::uint16_t spawn_fallback_delay_frames = 3;
		constexpr std::uint16_t max_actor_registration_wait_frames = 60;
		constexpr std::size_t max_npc_control_attempts_per_frame = 32;
		constexpr std::uint32_t isolation_maintenance_interval_frames = 15;
		constexpr std::size_t game_object_system_add_sink_slot = 16;
		constexpr std::size_t game_object_system_remove_sink_slot = 17;
		constexpr int entity_event_init = 3;
		constexpr int entity_event_script = 18;

		// Only the prefix read by OnBeforeSpawn is modeled here. The offsets are
		// pinned by CEntity's constructor: pClass is copied from +0x18 and the
		// stock/shipping spawn-param prefix places sName at +0x30.
		struct entity_spawn_params_prefix
		{
			std::uint32_t id{};
			std::uint32_t previous_id{};
			std::uint64_t guid{};
			std::uint64_t previous_guid{};
			void *entity_class{};
			void *archetype{};
			const char *layer_name{};
			const char *entity_name{};
		};
		static_assert(offsetof(entity_spawn_params_prefix, entity_class) == 0x18);
		static_assert(offsetof(entity_spawn_params_prefix, entity_name) == 0x30);

		struct entity_class_view
		{
			virtual ~entity_class_view() = default;
			virtual void Release() = 0;
			virtual const char *GetName() const = 0;
		};

		struct spawn_description
		{
			std::string class_name;
			std::string entity_name;
		};
		struct raw_spawn_description
		{
			const char *class_name{};
			const char *entity_name{};
		};

		struct native_entity_event
		{
			std::int32_t event{};
			std::int32_t padding{};
			std::intptr_t parameters[4]{};
			float floats[2]{};
		};

		std::uint64_t now_ms()
		{
			return static_cast<std::uint64_t>(
			    std::chrono::duration_cast<std::chrono::milliseconds>(
			        std::chrono::steady_clock::now().time_since_epoch())
			        .count());
		}

		raw_spawn_description guarded_read_spawn(
		    void *raw_params) noexcept
		{
			if (!raw_params)
				return {};
#ifdef _WIN32
			__try
			{
#endif
				const auto *params = static_cast<
				    const entity_spawn_params_prefix *>(raw_params);
				auto *entity_class = static_cast<entity_class_view *>(
				    params->entity_class);
				if (!entity_class)
					return {};
				const auto *class_name = entity_class->GetName();
				if (!class_name || class_name[0] == '\0')
					return {};
				return {class_name, params->entity_name};
#ifdef _WIN32
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return {};
			}
#endif
		}

		std::optional<spawn_description> describe_spawn(
		    void *raw_params) noexcept
		{
			const auto raw = guarded_read_spawn(raw_params);
			if (!raw.class_name)
				return std::nullopt;
			try
			{
				return spawn_description{
				    raw.class_name,
				    raw.entity_name ? raw.entity_name : ""};
			}
			catch (...)
			{
				return std::nullopt;
			}
		}

		protocol::Quaternion quaternion_from_matrix(const Matrix34 &matrix)
		{
			Quat rotation(matrix);
			protocol::Quaternion result;
			result.set_x(rotation.v.x);
			result.set_y(rotation.v.y);
			result.set_z(rotation.v.z);
			result.set_w(rotation.w);
			return result;
		}

		Matrix34 matrix_from_transform(
		    const protocol::TransformState &transform)
		{
			const Quat rotation(
			    transform.rotation().w(),
			    transform.rotation().x(),
			    transform.rotation().y(),
			    transform.rotation().z());
			return Matrix34(
			    Vec3(1.0F, 1.0F, 1.0F),
			    rotation,
			    Vec3(
			        transform.position().x(),
			        transform.position().y(),
			        transform.position().z()));
		}

		bool guarded_set_world_tm(
		    Offsets::IEntity *entity,
		    const Matrix34 *matrix) noexcept
		{
#ifdef _WIN32
			__try
			{
				entity->SetWorldTM(*matrix, 0);
				return true;
			}
			__except(KCD2MP_JOIN_SEH_FILTER(
			    "join.entity.SetWorldTM.seh"))
			{
				return false;
			}
#else
			entity->SetWorldTM(*matrix, 0);
			return true;
#endif
		}

		enum class actor_type_match
		{
			no,
			yes,
			failed
		};

		actor_type_match guarded_actor_type_matches(
		    wh::entitymodule::C_Actor *actor,
		    bool human) noexcept
		{
#ifdef _WIN32
			__try
			{
				return (human
				    ? actor->IsHumanActor()
				    : actor->IsAnimalActor())
				    ? actor_type_match::yes
				    : actor_type_match::no;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return actor_type_match::failed;
			}
#else
			return (human
			    ? actor->IsHumanActor()
			    : actor->IsAnimalActor())
			    ? actor_type_match::yes
			    : actor_type_match::no;
#endif
		}

		actor_type_match guarded_actor_is_player(
		    wh::entitymodule::C_Actor *actor) noexcept
		{
			if (!actor)
				return actor_type_match::no;
#ifdef _WIN32
			__try
			{
				return actor->IsPlayer()
				    ? actor_type_match::yes
				    : actor_type_match::no;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return actor_type_match::failed;
			}
#else
			return actor->IsPlayer()
			    ? actor_type_match::yes
			    : actor_type_match::no;
#endif
		}

		actor_type_match guarded_actor_owns_entity(
		    wh::entitymodule::C_Actor *actor,
		    Offsets::IEntity *entity) noexcept
		{
			if (!actor || !entity)
				return actor_type_match::no;
#ifdef _WIN32
			__try
			{
				return static_cast<void *>(actor->GetEntity())
				        == static_cast<void *>(entity)
				    ? actor_type_match::yes
				    : actor_type_match::no;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return actor_type_match::failed;
			}
#else
			return static_cast<void *>(actor->GetEntity())
			        == static_cast<void *>(entity)
			    ? actor_type_match::yes
			    : actor_type_match::no;
#endif
		}

		actor_type_match guarded_entity_has_ai(
		    Offsets::IEntity *entity) noexcept
		{
			if (!entity)
				return actor_type_match::no;
#ifdef _WIN32
			__try
			{
				return entity->HasAI()
				    ? actor_type_match::yes
				    : actor_type_match::no;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return actor_type_match::failed;
			}
#else
			return entity->HasAI()
			    ? actor_type_match::yes
			    : actor_type_match::no;
#endif
		}

		bool guarded_name_matches_named_cvar(
		    const char *entity_name,
		    const char *cvar_name) noexcept
		{
			if (!entity_name || entity_name[0] == '\0' || !cvar_name)
				return false;
			auto *environment = SSystemGlobalEnvironment::GetInstance();
			auto *console = environment ? environment->pConsole : nullptr;
			if (!console)
				return false;
#ifdef _WIN32
			__try
			{
#endif
				auto *cvar = console->GetCVar(cvar_name);
				if (!cvar)
					return false;
				const auto *protected_name = cvar->GetString();
				return protected_name && protected_name[0] != '\0'
				    && std::string_view(protected_name) == entity_name;
#ifdef _WIN32
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
#endif
		}

		bool guarded_entity_matches_named_cvar(
		    Offsets::IEntity *entity,
		    const char *cvar_name) noexcept
		{
			if (!entity)
				return false;
#ifdef _WIN32
			__try
			{
#endif
				return guarded_name_matches_named_cvar(
				    entity->GetName(), cvar_name);
#ifdef _WIN32
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
#endif
		}

		bool guarded_is_player_scheduler_proxy_name(
		    const char *entity_name) noexcept
		{
			return guarded_name_matches_named_cvar(
			           entity_name,
			           "wh_ai_PlayerSchedulerProxy")
			    || guarded_name_matches_named_cvar(
			        entity_name,
			        "wh_ai_PlayerHorseSchedulerProxy");
		}

		bool guarded_is_player_scheduler_proxy(
		    Offsets::IEntity *entity) noexcept
		{
			return guarded_entity_matches_named_cvar(
			           entity,
			           "wh_ai_PlayerSchedulerProxy")
			    || guarded_entity_matches_named_cvar(
			        entity,
			        "wh_ai_PlayerHorseSchedulerProxy");
		}

		bool guarded_apply_entity_isolation(
		    Offsets::IEntity *entity) noexcept
		{
			if (!entity)
				return false;
#ifdef _WIN32
			__try
			{
				entity->Hide(true);
				return true;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
#else
			entity->Hide(true);
			return true;
#endif
		}

		bool guarded_restore_entity(
		    Offsets::IEntity *entity,
		    bool hidden) noexcept
		{
			if (!entity)
				return false;
#ifdef _WIN32
			__try
			{
				entity->Hide(hidden);
				return true;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
#else
			entity->Hide(hidden);
			return true;
#endif
		}

		bool guarded_entity_visible(Offsets::IEntity *entity) noexcept
		{
			if (!entity)
				return false;
#ifdef _WIN32
			__try
			{
				return !entity->IsHidden();
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
#else
			return !entity->IsHidden();
#endif
		}

		bool guarded_is_actor_class(
		    Offsets::IActorSystem *actor_system,
		    void *entity_class) noexcept
		{
#ifdef _WIN32
			__try
			{
				return actor_system->IsActorClass(
				    reinterpret_cast<IEntityClass *>(entity_class));
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
#else
			return actor_system->IsActorClass(
			    reinterpret_cast<IEntityClass *>(entity_class));
#endif
		}

		int guarded_actor_count(Offsets::IActorSystem *actor_system) noexcept
		{
			if (!actor_system)
				return -1;
#ifdef _WIN32
			__try
			{
				return actor_system->GetActorCount();
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return -1;
			}
#else
			return actor_system->GetActorCount();
#endif
		}

		std::uint32_t guarded_game_object_entity_id(void *game_object) noexcept
		{
			if (!game_object)
				return 0;
#ifdef _WIN32
			__try
			{
				// IGameObject inherits IActionListener. Its first data member,
				// m_entityId, follows that base's vptr at +0x08 in the stock and
				// shipping KCD2 interface layout.
				return *reinterpret_cast<const std::uint32_t *>(
				    static_cast<const std::byte *>(game_object) + 0x08);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return 0;
			}
#else
			return *reinterpret_cast<const std::uint32_t *>(
			    static_cast<const std::byte *>(game_object) + 0x08);
#endif
		}

		bool guarded_game_object_sink_call(
		    void *system,
		    std::size_t slot,
		    void *sink) noexcept
		{
			if (!system || !sink)
				return false;
#ifdef _WIN32
			__try
			{
				auto **vtable = *reinterpret_cast<void ***>(system);
				if (!vtable || !vtable[slot])
					return false;
				using Fn = void (__fastcall *)(void *, void *);
				reinterpret_cast<Fn>(vtable[slot])(system, sink);
				return true;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				return false;
			}
#else
			auto **vtable = *reinterpret_cast<void ***>(system);
			if (!vtable || !vtable[slot])
				return false;
			using Fn = void (*)(void *, void *);
			reinterpret_cast<Fn>(vtable[slot])(system, sink);
			return true;
#endif
		}
	}

	void native_entity_backend::isolation_sink::attach(
	    native_entity_backend &owner)
	{
		m_owner = &owner;
	}

	bool native_entity_backend::isolation_sink::OnBeforeSpawn(void *params)
	{
		return !m_owner || m_owner->allow_human_npc_spawn(params);
	}

	void native_entity_backend::isolation_sink::OnSpawn(
	    Offsets::IEntity *entity,
	    void *)
	{
		m_owner->queue_entity_for_control(entity, false, false);
	}

	bool native_entity_backend::isolation_sink::OnRemove(
	    Offsets::IEntity *entity)
	{
		m_owner->entity_removed(entity);
		return true;
	}

	void native_entity_backend::isolation_sink::OnReused(
	    Offsets::IEntity *entity,
	    void *)
	{
		m_owner->entity_removed(entity);
		m_owner->queue_entity_for_control(entity, false, false);
	}

	void native_entity_backend::isolation_sink::_vf5(
	    Offsets::IEntity *,
	    void *)
	{
	}

	void native_entity_backend::isolation_sink::OnEvent(
	    Offsets::IEntity *entity,
	    void *event)
	{
		m_owner->entity_event(entity, event);
	}

	void native_entity_backend::isolation_sink::GetMemoryUsage(void *) const
	{
	}

	void native_entity_backend::game_object_init_sink::attach(
	    native_entity_backend &owner)
	{
		m_owner = &owner;
	}

	void native_entity_backend::game_object_init_sink::OnAfterInit(
	    void *game_object)
	{
		const auto id = guarded_game_object_entity_id(game_object);
		if (m_owner && id != 0)
			m_owner->game_object_initialized(id);
	}

	native_entity_backend::native_entity_backend()
	{
		m_sink.attach(*this);
		m_game_object_sink.attach(*this);
	}

	native_entity_backend::human_npc_spawn_scope::human_npc_spawn_scope(
	    native_entity_backend &owner,
	    std::uint64_t token) noexcept
	    : m_owner(&owner), m_token(token)
	{
	}

	native_entity_backend::human_npc_spawn_scope::human_npc_spawn_scope(
	    human_npc_spawn_scope &&other) noexcept
	    : m_owner(other.m_owner), m_token(other.m_token)
	{
		other.m_owner = nullptr;
		other.m_token = 0;
	}

	native_entity_backend::human_npc_spawn_scope &
	native_entity_backend::human_npc_spawn_scope::operator=(
	    human_npc_spawn_scope &&other) noexcept
	{
		if (this == &other)
			return *this;
		if (m_owner)
			m_owner->end_human_npc_spawn_authorization(m_token);
		m_owner = other.m_owner;
		m_token = other.m_token;
		other.m_owner = nullptr;
		other.m_token = 0;
		return *this;
	}

	native_entity_backend::human_npc_spawn_scope::~human_npc_spawn_scope()
	{
		if (m_owner)
			m_owner->end_human_npc_spawn_authorization(m_token);
	}

	native_entity_backend::~native_entity_backend()
	{
		if (m_game_object_system)
		{
			(void)guarded_game_object_sink_call(
			    m_game_object_system,
			    game_object_system_remove_sink_slot,
			    &m_game_object_sink);
		}
		if (m_sink_system)
			m_sink_system->RemoveSink(&m_sink);
	}

	native_player_view native_entity_backend::player() const
	{
		auto *framework = CCryAction::GetInstance();
		auto *entity = framework ? framework->GetClientEntity() : nullptr;
		auto *context = wh::game::S_GameContext::GetInstance();
		KCD2MP_JOIN_TRACE(
		    "join.local-player.state",
		    std::format(
		        "framework={} client_entity={} game_context={} "
		        "actor_system={} entity_system={} thread_role={}",
		        static_cast<void *>(framework),
		        static_cast<void *>(entity),
		        static_cast<void *>(context),
		        context
		            ? static_cast<void *>(context->m_pActorSystem)
		            : nullptr,
		        SSystemGlobalEnvironment::GetInstance()
		                ? static_cast<void *>(
		                      SSystemGlobalEnvironment::GetInstance()
		                          ->pEntitySystem)
		                : nullptr,
		        join_trace::thread_role_name(
		            join_trace::current_thread_role())));
		if (!context || !entity)
		{
			KCD2MP_JOIN_TRACE(
			    "join.local-player.null",
			    std::format(
			        "context_null={} entity_null={}",
			        context == nullptr,
			        entity == nullptr));
		}
		auto *actor =
		    context && entity ? context->GetActorById(entity->GetId()) : nullptr;
		KCD2MP_JOIN_TRACE(
		    actor ? "join.local-actor.resolved"
		          : "join.local-actor.null",
		    std::format(
		        "entity={} actor={}",
		        static_cast<void *>(entity),
		        static_cast<void *>(actor)));
		return {entity, actor};
	}

	std::optional<protocol::TransformState>
	native_entity_backend::read_transform(Offsets::IEntity *entity) const
	{
		if (!entity)
		{
			KCD2MP_JOIN_TRACE(
			    "join.entity.read-transform.skipped",
			    "entity=nil");
			return std::nullopt;
		}
		KCD2MP_JOIN_TRACE(
		    "join.entity.GetWorldTMPtr.begin",
		    std::format("entity={}", static_cast<void *>(entity)));
		const auto *matrix = entity->GetWorldTMPtr();
		if (!matrix)
		{
			KCD2MP_JOIN_TRACE(
			    "join.entity.GetWorldTMPtr.nil",
			    std::format("entity={}", static_cast<void *>(entity)));
			return std::nullopt;
		}
		protocol::TransformState result;
		result.mutable_position()->set_x(matrix->m03);
		result.mutable_position()->set_y(matrix->m13);
		result.mutable_position()->set_z(matrix->m23);
		*result.mutable_rotation() = quaternion_from_matrix(*matrix);
		result.mutable_velocity();
		result.set_client_time_ms(now_ms());
		return result;
	}

	bool native_entity_backend::write_transform(
	    Offsets::IEntity *entity,
	    const protocol::TransformState &transform,
	    std::string &error) const
	{
		if (!entity || !is_finite_transform(transform))
		{
			error = "native transform target or entity is invalid";
			KCD2MP_JOIN_TRACE(
			    "join.entity.write-transform.rejected",
			    std::format(
			        "entity={} finite={} error=\"{}\"",
			        static_cast<void *>(entity),
			        is_finite_transform(transform),
			        error));
			return false;
		}
		const auto matrix = matrix_from_transform(transform);
		KCD2MP_JOIN_TRACE(
		    "join.entity.SetWorldTM.begin",
		    std::format(
		        "entity={} position=({:.6f},{:.6f},{:.6f}) "
		        "rotation=({:.6f},{:.6f},{:.6f},{:.6f})",
		        static_cast<void *>(entity),
		        transform.position().x(),
		        transform.position().y(),
		        transform.position().z(),
		        transform.rotation().x(),
		        transform.rotation().y(),
		        transform.rotation().z(),
		        transform.rotation().w()));
		if (!guarded_set_world_tm(entity, &matrix))
		{
			error = "SEH exception in IEntity::SetWorldTM";
			KCD2MP_JOIN_TRACE(
			    "join.entity.SetWorldTM.failed",
			    error);
			return false;
		}
		KCD2MP_JOIN_TRACE(
		    "join.entity.SetWorldTM.returned",
		    std::format("entity={}", static_cast<void *>(entity)));
		const auto verified = read_transform(entity);
		if (!verified)
		{
			error = "SetWorldTM did not leave a readable matrix";
			KCD2MP_JOIN_TRACE(
			    "join.entity.SetWorldTM.verify-failed",
			    error);
			return false;
		}
		const auto &actual = verified->position();
		const auto &desired = transform.position();
		const auto position_error = std::hypot(
		    std::hypot(actual.x() - desired.x(), actual.y() - desired.y()),
		    actual.z() - desired.z());
		const auto dot = std::abs(
		    verified->rotation().x() * transform.rotation().x()
		    + verified->rotation().y() * transform.rotation().y()
		    + verified->rotation().z() * transform.rotation().z()
		    + verified->rotation().w() * transform.rotation().w());
		if (position_error > 0.01F || dot < 0.9999F)
		{
			error = "SetWorldTM readback exceeded position/rotation tolerance";
			KCD2MP_JOIN_TRACE(
			    "join.entity.SetWorldTM.verify-failed",
			    std::format(
			        "position_error={} quaternion_dot={} error=\"{}\"",
			        position_error,
			        dot,
			        error));
			return false;
		}
		KCD2MP_JOIN_TRACE(
		    "join.entity.SetWorldTM.ok",
		    std::format(
		        "position_error={} quaternion_dot={}",
		        position_error,
		        dot));
		return true;
	}

	bool native_entity_backend::set_world_isolated(
	    bool humans_disabled,
	    bool animals_disabled,
	    std::string &error)
	{
		if (m_human_npcs_disabled == humans_disabled
		    && m_animal_npcs_disabled == animals_disabled)
		{
			return true;
		}
		restore_world();
		if (!humans_disabled && !animals_disabled)
		{
			return true;
		}
		const auto local = player();
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		auto *system = environment ? environment->pEntitySystem : nullptr;
		KCD2MP_JOIN_TRACE(
		    "join.entity-isolation.precheck",
		    std::format(
		        "humans_disabled={} animals_disabled={} local_entity={} "
		        "environment={} entity_system={}",
		        humans_disabled,
		        animals_disabled,
		        static_cast<void *>(local.entity),
		        static_cast<void *>(environment),
		        static_cast<void *>(system)));
		if (!local.entity || !system)
		{
			error = "entity isolation requires the local player and entity system";
			KCD2MP_JOIN_TRACE(
			    "join.entity-isolation.failed",
			    error);
			return false;
		}
		ensure_sink_registered(*system);
		auto *framework = CCryAction::GetInstance();
		ensure_game_object_sink_registered(
		    framework ? framework->m_pGameObjectSystem : nullptr);
		m_local_player_entity_id = local.entity->GetId();
		m_human_npcs_disabled = humans_disabled;
		m_animal_npcs_disabled = animals_disabled;
		m_isolation_active = true;
		m_isolation_maintenance_frame = 0;
		auto *context = wh::game::S_GameContext::GetInstance();
		m_last_actor_count = guarded_actor_count(
		    context ? context->m_pActorSystem : nullptr);
		auto *iterator = system->GetEntityIterator();
		if (!iterator)
		{
			m_isolation_active = false;
			m_local_player_entity_id = 0;
			m_human_npcs_disabled = false;
			m_animal_npcs_disabled = false;
			error = "entity system did not create an iterator";
			KCD2MP_JOIN_TRACE(
			    "join.entity-isolation.failed",
			    error);
			return false;
		}
		// GetEntityIterator returns the ref-counted CEntityItMap interface. The
		// established enumeration path AddRefs it, calls MoveFirst, and advances
		// until Next returns null. Bound the walk by the entity count captured
		// before isolation so a malformed or mutation-sensitive iterator cannot
		// loop forever on the PostUpdate frame.
		iterator->AddRef();
		const auto entity_count = system->GetNumEntities();
		std::uint32_t visited{};
		iterator->MoveFirst();
		for (; visited < entity_count; ++visited)
		{
			auto *entity = iterator->Next();
			if (!entity)
				break;
			queue_entity_for_control(entity, true, true);
		}
		iterator->Release();
		process_pending_entity_control();
		KCD2MP_JOIN_TRACE(
		    "join.entity-isolation.complete",
		    std::format(
		        "reported={} visited={} isolated={} pending={}",
		        entity_count,
		        visited,
		        m_isolated.size(),
		        m_pending_control.size()));
		return true;
	}

	void native_entity_backend::register_player_entity(
	    std::uint32_t entity_id)
	{
		if (entity_id != 0)
		{
			m_player_entities.insert(entity_id);
			m_pending_control.erase(entity_id);
			if (const auto isolated = m_isolated.find(entity_id);
			    isolated != m_isolated.end())
			{
				auto *environment = SSystemGlobalEnvironment::GetInstance();
				auto *system = environment ? environment->pEntitySystem : nullptr;
				if (system)
				{
					(void)guarded_restore_entity(
					    system->GetEntity(entity_id),
					    isolated->second.hidden);
				}
				m_isolated.erase(isolated);
			}
		}
	}

	void native_entity_backend::unregister_player_entity(
	    std::uint32_t entity_id)
	{
		m_player_entities.erase(entity_id);
	}

	native_entity_backend::human_npc_spawn_scope
	native_entity_backend::authorize_human_npc_spawn(
	    std::string entity_name)
	{
		++m_next_human_npc_spawn_token;
		if (m_next_human_npc_spawn_token == 0)
			++m_next_human_npc_spawn_token;
		m_human_npc_spawn_authorizations.push_back(
		    human_npc_spawn_authorization{
		        m_next_human_npc_spawn_token,
		        std::move(entity_name),
		        std::this_thread::get_id(),
		        false});
		return human_npc_spawn_scope{
		    *this, m_next_human_npc_spawn_token};
	}

	void native_entity_backend::end_human_npc_spawn_authorization(
	    std::uint64_t token)
	{
		for (auto it = m_human_npc_spawn_authorizations.begin();
		     it != m_human_npc_spawn_authorizations.end(); ++it)
		{
			if (it->token == token)
			{
				m_human_npc_spawn_authorizations.erase(it);
				return;
			}
		}
	}

	bool native_entity_backend::managed_human_spawn_active() const
	{
		const auto thread = std::this_thread::get_id();
		for (const auto &authorization : m_human_npc_spawn_authorizations)
		{
			if (authorization.thread == thread)
				return true;
		}
		return false;
	}

	bool native_entity_backend::allow_human_npc_spawn(void *params)
	{
		if (!m_isolation_active || !m_human_npcs_disabled)
			return true;

		const auto spawn = describe_spawn(params);
		if (!spawn)
		{
			join_trace::write_diagnostic(
			    "entity-control.spawn.unclassified",
			    "spawn params or entity class name could not be read; allowing");
			return true;
		}
		if (spawn->class_name != "NPC"
		    && spawn->class_name != "NPC_Female")
		{
			return true;
		}
		if (guarded_is_player_scheduler_proxy_name(
		        spawn->entity_name.c_str()))
		{
			join_trace::write_diagnostic(
			    "entity-control.spawn.protected",
			    std::format(
			        "class=\"{}\" name=\"{}\" reason=player-scheduler-proxy",
			        spawn->class_name,
			        spawn->entity_name));
			return true;
		}

		const auto thread = std::this_thread::get_id();
		for (auto it = m_human_npc_spawn_authorizations.rbegin();
		     it != m_human_npc_spawn_authorizations.rend(); ++it)
		{
			if (!it->consumed && it->thread == thread
			    && it->entity_name == spawn->entity_name)
			{
				it->consumed = true;
				join_trace::write_diagnostic(
				    "entity-control.spawn.authorized",
				    std::format(
				        "class=\"{}\" name=\"{}\" token={}",
				        spawn->class_name,
				        spawn->entity_name,
				        it->token));
				return true;
			}
		}

		join_trace::write_diagnostic(
		    "entity-control.spawn.blocked",
		    std::format(
		        "class=\"{}\" name=\"{}\" reason=not-kcd2mp-authorized",
		        spawn->class_name,
		        spawn->entity_name));
		return false;
	}

	void native_entity_backend::process_pending_entity_control()
	{
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		auto *system = environment ? environment->pEntitySystem : nullptr;
		if (!system)
			return;
		m_world_sync.process();
		m_world_item_sync.process();
		if (!m_isolation_active || managed_human_spawn_active())
			return;
		refresh_local_player_exclusion(*system);
		++m_isolation_maintenance_frame;
		if (m_isolation_maintenance_frame
		        % isolation_maintenance_interval_frames
		    == 0)
		{
			maintain_isolated_entities(*system);
		}
		refresh_actor_roster(*system);
		if (m_pending_control.empty())
			return;

		std::vector<std::pair<std::uint32_t, pending_entity>> pending;
		pending.reserve(m_pending_control.size());
		for (const auto entry : m_pending_control)
			pending.push_back(entry);
		m_pending_control.clear();
		auto *context = wh::game::S_GameContext::GetInstance();
		std::size_t control_attempts{};
		for (const auto &[id, state] : pending)
		{
			if (control_attempts >= max_npc_control_attempts_per_frame)
			{
				m_pending_control.emplace(id, state);
				continue;
			}
			auto *entity = system->GetEntity(id);
			if (!entity)
				continue;
			if (!state.game_object_initialized
			    && state.waited_frames < spawn_fallback_delay_frames)
			{
				m_pending_control.emplace(
				    id,
				    pending_entity{
				        static_cast<std::uint16_t>(state.waited_frames + 1),
				        false});
				continue;
			}
			const auto actor = context ? context->GetActorById(id) : nullptr;
			if (!actor)
			{
				auto *actor_system = context ? context->m_pActorSystem : nullptr;
				const auto actor_class = actor_system && entity->GetClass()
				    && guarded_is_actor_class(
				        actor_system,
				        entity->GetClass());
				if (actor_class
				    && state.waited_frames
				        < max_actor_registration_wait_frames)
				{
					m_pending_control.emplace(
					    id,
					    pending_entity{
					        static_cast<std::uint16_t>(state.waited_frames + 1),
					        state.game_object_initialized});
				}
				continue;
			}
			++control_attempts;
			(void)isolate_npc_entity(entity);
		}
	}

	bool native_entity_backend::begin_world_sync(std::string &error)
	{
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		auto *system = environment ? environment->pEntitySystem : nullptr;
		if (!system)
		{
			error = "world interaction sync requires the entity system";
			return false;
		}
		ensure_sink_registered(*system);
		m_world_sync.reset();
		if (!m_world_item_sync.begin(error))
			return false;
		error.clear();
		return true;
	}

	std::vector<protocol::WorldObjectState>
	native_entity_backend::poll_world_object_updates()
	{
		return m_world_sync.poll_updates();
	}

	bool native_entity_backend::apply_world_object_state(
	    const protocol::WorldObjectState &state,
	    std::string &error)
	{
		return m_world_sync.apply(state, error);
	}

	std::vector<protocol::WorldItemState>
	native_entity_backend::poll_world_item_updates()
	{
		return m_world_item_sync.poll_updates();
	}

	bool native_entity_backend::apply_world_item_state(
	    const protocol::WorldItemState &state,
	    std::string &error)
	{
		return m_world_item_sync.apply(state, error);
	}

	void native_entity_backend::reset_world_sync()
	{
		m_world_sync.reset();
		m_world_item_sync.reset();
	}

	void native_entity_backend::restore_world()
	{
		reset_world_sync();
		m_isolation_active = false;
		m_local_player_entity_id = 0;
		m_human_npcs_disabled = false;
		m_animal_npcs_disabled = false;
		m_isolation_maintenance_frame = 0;
		m_last_actor_count = -1;
		m_pending_control.clear();
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		auto *system = environment ? environment->pEntitySystem : nullptr;
		if (system)
		{
			for (const auto &[id, state] : m_isolated)
			{
				(void)guarded_restore_entity(
				    system->GetEntity(id),
				    state.hidden);
			}
		}
		m_isolated.clear();
	}

	void native_entity_backend::queue_entity_for_control(
	    Offsets::IEntity *entity,
	    bool game_object_initialized,
	    bool actor_class_confirmed)
	{
		if (!m_isolation_active || !entity || managed_human_spawn_active())
			return;
		const auto id = entity->GetId();
		// Spawn callbacks run before actor-extension registration on some
		// streamed NPC paths. Do not classify them here; the PostUpdate queue
		// discards ordinary entities after the short initialization delay.
		if (actor_class_confirmed)
		{
			auto *context = wh::game::S_GameContext::GetInstance();
			auto *actor_system = context ? context->m_pActorSystem : nullptr;
			if (!actor_system || !entity->GetClass()
			    || !guarded_is_actor_class(actor_system, entity->GetClass()))
			{
				return;
			}
		}
		if (id != 0 && id != m_local_player_entity_id
		    && !m_player_entities.contains(id)
		    && !m_isolated.contains(id))
		{
			auto [it, inserted] = m_pending_control.try_emplace(
			    id,
			    pending_entity{});
			(void)inserted;
			it->second.game_object_initialized |= game_object_initialized;
		}
	}

	void native_entity_backend::game_object_initialized(
	    std::uint32_t entity_id)
	{
		if (!m_isolation_active || entity_id == 0)
			return;
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		auto *system = environment ? environment->pEntitySystem : nullptr;
		if (system)
		{
			if (auto *entity = system->GetEntity(entity_id))
				queue_entity_for_control(entity, true, false);
		}
	}

	void native_entity_backend::refresh_actor_roster(
	    Offsets::IEntitySystem &system)
	{
		auto *context = wh::game::S_GameContext::GetInstance();
		auto *actor_system = context ? context->m_pActorSystem : nullptr;
		const auto actor_count = guarded_actor_count(actor_system);
		if (actor_count < 0 || actor_count == m_last_actor_count)
			return;
		m_last_actor_count = actor_count;

		auto *iterator = system.GetEntityIterator();
		if (!iterator)
			return;
		iterator->AddRef();
		const auto entity_count = system.GetNumEntities();
		iterator->MoveFirst();
		for (std::uint32_t visited{}; visited < entity_count; ++visited)
		{
			auto *entity = iterator->Next();
			if (!entity)
				break;
			queue_entity_for_control(entity, true, true);
		}
		iterator->Release();
	}

	void native_entity_backend::refresh_local_player_exclusion(
	    Offsets::IEntitySystem &)
	{
		auto *framework = CCryAction::GetInstance();
		auto *entity = framework ? framework->GetClientEntity() : nullptr;
		if (!entity)
			return;
		const auto current_id = entity->GetId();
		if (current_id == 0)
			return;
		m_local_player_entity_id = current_id;
		m_pending_control.erase(current_id);
		if (const auto isolated = m_isolated.find(current_id);
		    isolated != m_isolated.end())
		{
			(void)guarded_restore_entity(
			    entity,
			    isolated->second.hidden);
			m_isolated.erase(isolated);
		}
	}

	void native_entity_backend::maintain_isolated_entities(
	    Offsets::IEntitySystem &system)
	{
		for (auto iterator = m_isolated.begin();
		     iterator != m_isolated.end();)
		{
			const auto id = iterator->first;
			const auto state = iterator->second;
			auto *entity = system.GetEntity(id);
			if (id == m_local_player_entity_id
			    || m_player_entities.contains(id)
			    || !should_isolate_npc_actor(entity))
			{
				if (entity)
				{
					(void)guarded_restore_entity(
					    entity,
					    state.hidden);
				}
				iterator = m_isolated.erase(iterator);
				continue;
			}
			if (guarded_entity_visible(entity))
				(void)guarded_apply_entity_isolation(entity);
			++iterator;
		}
	}

	void native_entity_backend::ensure_sink_registered(
	    Offsets::IEntitySystem &system)
	{
		if (m_sink_system == &system)
			return;
		if (m_sink_system)
			m_sink_system->RemoveSink(&m_sink);
		m_sink_system = &system;
		constexpr std::uint32_t before_spawn_remove_reuse_subscriptions =
		    (1U << 0) | (1U << 1) | (1U << 2) | (1U << 3);
		constexpr std::uint64_t entity_event_subscriptions =
		    (1ULL << entity_event_init) | (1ULL << entity_event_script);
		system.AddSink(
		    &m_sink,
		    before_spawn_remove_reuse_subscriptions,
		    entity_event_subscriptions);
	}

	void native_entity_backend::ensure_game_object_sink_registered(
	    void *system)
	{
		if (m_game_object_system == system)
			return;
		if (m_game_object_system)
		{
			(void)guarded_game_object_sink_call(
			    m_game_object_system,
			    game_object_system_remove_sink_slot,
			    &m_game_object_sink);
			m_game_object_system = nullptr;
		}
		if (system && guarded_game_object_sink_call(
		        system,
		        game_object_system_add_sink_slot,
		        &m_game_object_sink))
		{
			m_game_object_system = system;
		}
	}

	bool native_entity_backend::isolate_npc_entity(Offsets::IEntity *entity)
	{
		if (!m_isolation_active || !entity || managed_human_spawn_active())
			return false;
		const auto id = entity->GetId();
		if (id == m_local_player_entity_id
		    || m_player_entities.contains(id)
		    || m_isolated.contains(id)
		    || !should_isolate_npc_actor(entity))
			return false;
		auto *context = wh::game::S_GameContext::GetInstance();
		auto *actor = context ? context->GetActorById(id) : nullptr;
		if (actor
		    && read_combat_state(actor->m_pCombatActor).combat_mode)
		{
			// Let the native CombatScene finish its opponent and end-combat
			// transitions before suspending the NPC. Hiding a live opponent is what
			// previously left the player in a permanent raised-hands state.
			m_pending_control.try_emplace(
			    id,
			    pending_entity{max_actor_registration_wait_frames, true});
			KCD2MP_JOIN_TRACE(
			    "join.entity-control.deferred",
			    std::format(
			        "entity_id={} reason=native-combat-active",
			        id));
			return false;
		}

		// Keep the Actor, Soul, scheduler and RandomEvent ownership intact. Vanilla
		// systems retain GUID references to streamed NPCs and dereference them on
		// later ticks; force-removing the Entity leaves those references dangling.
		// Hiding only the positively classified NPC preserves that ownership graph
		// while excluding it from rendering and normal entity updates.
		const entity_state state{entity->IsHidden()};
		join_trace::write_diagnostic(
		    "entity-control.isolate.begin",
		    std::format(
		        "entity_id={} local_entity_id={} registered_players={}",
		        id,
		        m_local_player_entity_id,
		        m_player_entities.size()));
		if (!guarded_apply_entity_isolation(entity))
		{
			(void)guarded_restore_entity(entity, state.hidden);
			join_trace::write_diagnostic(
			    "entity-control.isolate.failed",
			    std::format("entity_id={}", id));
			return false;
		}

		m_isolated.emplace(id, state);
		join_trace::write_diagnostic(
		    "entity-control.isolate.complete",
		    std::format(
		        "entity_id={} isolated_total={} actor_preserved=true",
		        id,
		        m_isolated.size()));
		return true;
	}

	bool native_entity_backend::should_isolate_npc_actor(
	    Offsets::IEntity *entity) const
	{
		if (!entity)
			return false;
		const auto entity_id = entity->GetId();
		auto *context = wh::game::S_GameContext::GetInstance();
		auto *actor = context ? context->GetActorById(entity_id) : nullptr;
		if (!actor)
			return false;
		// The actor-map entry must own this exact entity. This rejects stale or
		// reused ids before any runtime-type or AI probe is trusted.
		if (guarded_actor_owns_entity(actor, entity)
		    != actor_type_match::yes)
		{
			return false;
		}
		auto *framework = CCryAction::GetInstance();
		auto *client_entity = framework ? framework->GetClientEntity() : nullptr;
		if (client_entity
		    && static_cast<void *>(client_entity)
		        == static_cast<void *>(entity))
		{
			return false;
		}
		auto *client_actor = framework ? framework->GetClientActor() : nullptr;
		if (client_actor
		    && static_cast<void *>(client_actor)
		        == static_cast<void *>(actor))
		{
			return false;
		}
		const auto player = guarded_actor_is_player(actor);
		if (player != actor_type_match::no)
		{
			// A positive result protects every engine-recognized player. A failed
			// player probe also stays active: NPC isolation must never risk
			// disabling input, combat, inventory, camera, or player controllers.
			return false;
		}
		if (guarded_is_player_scheduler_proxy(entity))
		{
			// KCD models the scheduler that drives player state/animation through
			// special AI-backed proxy Entities. They can derive from C_Human without
			// being IsPlayer(), so the ordinary Human+HasAI rule must not touch them.
			// Suspending this proxy leaves player action transitions and
			// player-relative MonsterLOD processing stuck.
			KCD2MP_JOIN_TRACE(
			    "join.entity-control.protected",
			    std::format(
			        "entity_id={} reason=player-scheduler-proxy",
			        entity_id));
			return false;
		}
		const auto human = guarded_actor_type_matches(actor, true);
		if (human == actor_type_match::yes)
		{
			// C_Human is also the base of C_Player and potentially other human
			// gameplay actors. A real NPC must additionally own an AI object;
			// animation, combat and other system actors do not.
			return m_human_npcs_disabled
			    && guarded_entity_has_ai(entity)
			        == actor_type_match::yes;
		}
		if (human == actor_type_match::failed)
			return false;

		const auto animal = guarded_actor_type_matches(actor, false);
		if (animal == actor_type_match::yes)
			return m_animal_npcs_disabled;
		if (animal == actor_type_match::failed)
			return false;

		// Unknown Actor subclasses stay active. Disabling a class that is not
		// proven to derive from C_Human or C_Animal can also suspend gameplay
		// helpers such as combat/system actors.
		return false;
	}

	void native_entity_backend::entity_event(
	    Offsets::IEntity *entity,
	    void *raw_event)
	{
		if (!entity || !raw_event)
			return;
		if (m_world_sync.handle_entity_event(entity, raw_event))
			return;
		const auto *event = static_cast<const native_entity_event *>(raw_event);
		if (event->event == entity_event_init)
		{
			queue_entity_for_control(entity, true, false);
		}
	}

	void native_entity_backend::entity_removed(Offsets::IEntity *entity)
	{
		if (!entity)
			return;
		const auto id = entity->GetId();
		m_isolated.erase(id);
		m_player_entities.erase(id);
		m_pending_control.erase(id);
		m_world_sync.entity_removed(entity);
	}
}
