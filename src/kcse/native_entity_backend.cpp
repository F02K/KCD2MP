#include "kcse/native_entity_backend.hpp"
#include "multiplayer/protocol.hpp"

#include <crysystem/CCryAction.h>
#include <crysystem/CEntity.h>
#include <crysystem/SSystemGlobalEnvironment.h>
#include <entitymodule/C_Actor.h>
#include <game/S_GameContext.h>
#include <Offsets/vtables/IEntity.h>
#include <Offsets/vtables/IEntityIt.h>
#include <Offsets/vtables/IEntitySystem.h>

#include <chrono>
#include <cmath>

namespace kcd2mp::kcse
{
	namespace
	{
		constexpr std::uint32_t entity_flag_calc_physics = 1U << 7;
		constexpr std::uint32_t entity_flag_has_ai = 1U << 13;
		constexpr std::uint32_t entity_flag_trigger_areas = 1U << 14;
		constexpr std::uint32_t entity_flag_no_save = 1U << 15;
		constexpr std::uint32_t entity_flag_clientside_state = 1U << 17;
		constexpr std::uint32_t entity_flag_no_proximity = 1U << 19;

		std::uint64_t now_ms()
		{
			return static_cast<std::uint64_t>(
			    std::chrono::duration_cast<std::chrono::milliseconds>(
			        std::chrono::steady_clock::now().time_since_epoch())
			        .count());
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
	}

	void native_entity_backend::isolation_sink::attach(
	    native_entity_backend &owner)
	{
		m_owner = &owner;
	}

	bool native_entity_backend::isolation_sink::OnBeforeSpawn(void *)
	{
		return true;
	}

	void native_entity_backend::isolation_sink::OnSpawn(
	    Offsets::IEntity *entity,
	    void *)
	{
		m_owner->isolate_entity(entity);
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
		m_owner->isolate_entity(entity);
	}

	void native_entity_backend::isolation_sink::_vf5(
	    Offsets::IEntity *,
	    void *)
	{
	}

	void native_entity_backend::isolation_sink::OnEvent(
	    Offsets::IEntity *,
	    void *)
	{
	}

	void native_entity_backend::isolation_sink::GetMemoryUsage(void *) const
	{
	}

	native_entity_backend::native_entity_backend()
	{
		m_sink.attach(*this);
	}

	native_entity_backend::~native_entity_backend()
	{
		if (m_sink_system)
			m_sink_system->RemoveSink(&m_sink);
	}

	native_player_view native_entity_backend::player() const
	{
		auto *framework = CCryAction::GetInstance();
		auto *entity = framework ? framework->GetClientEntity() : nullptr;
		auto *context = wh::game::S_GameContext::GetInstance();
		auto *actor =
		    context && entity ? context->GetActorById(entity->GetId()) : nullptr;
		return {entity, actor};
	}

	std::optional<protocol::TransformState>
	native_entity_backend::read_transform(Offsets::IEntity *entity) const
	{
		if (!entity)
			return std::nullopt;
		const auto *matrix = entity->GetWorldTMPtr();
		if (!matrix)
			return std::nullopt;
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
			return false;
		}
		const auto matrix = matrix_from_transform(transform);
		entity->SetWorldTM(matrix, 0);
		const auto verified = read_transform(entity);
		if (!verified)
		{
			error = "SetWorldTM did not leave a readable matrix";
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
			return false;
		}
		return true;
	}

	bool native_entity_backend::set_world_isolated(
	    bool disabled,
	    std::string &error)
	{
		if (!disabled)
		{
			restore_world();
			return true;
		}
		const auto local = player();
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		auto *system = environment ? environment->pEntitySystem : nullptr;
		if (!local.entity || !system)
		{
			error = "entity isolation requires the local player and entity system";
			return false;
		}
		ensure_sink_registered(*system);
		m_isolation_active = true;
		auto *iterator = system->GetEntityIterator();
		if (!iterator)
		{
			error = "entity system did not create an iterator";
			return false;
		}
		iterator->MoveFirst();
		while (!iterator->IsEnd())
		{
			auto *entity = iterator->Next();
			if (!entity || entity == local.entity
			    || m_player_entities.contains(entity->GetId())
			    || m_isolated.contains(entity->GetId()))
				continue;
			isolate_entity(entity);
		}
		iterator->Release();
		return true;
	}

	void native_entity_backend::register_player_entity(
	    std::uint32_t entity_id)
	{
		if (entity_id != 0)
		{
			m_player_entities.insert(entity_id);
			if (const auto it = m_isolated.find(entity_id);
			    it != m_isolated.end())
			{
				auto *environment = SSystemGlobalEnvironment::GetInstance();
				auto *system =
				    environment ? environment->pEntitySystem : nullptr;
				if (system)
				{
					if (auto *entity = system->GetEntity(entity_id))
					{
						entity->SetFlags(it->second.flags);
						entity->SetAIObjectID(it->second.ai_object_id);
						reinterpret_cast<CEntity *>(entity)->EnablePhysics(
						    (it->second.flags & entity_flag_calc_physics) != 0);
						entity->Hide(it->second.hidden);
						entity->Activate(it->second.active);
					}
				}
				m_isolated.erase(it);
			}
		}
	}

	void native_entity_backend::unregister_player_entity(
	    std::uint32_t entity_id)
	{
		m_player_entities.erase(entity_id);
	}

	void native_entity_backend::begin_player_spawn()
	{
		++m_player_spawn_depth;
	}

	void native_entity_backend::end_player_spawn()
	{
		if (m_player_spawn_depth != 0)
			--m_player_spawn_depth;
	}

	void native_entity_backend::restore_world()
	{
		m_isolation_active = false;
		auto *environment = SSystemGlobalEnvironment::GetInstance();
		auto *system = environment ? environment->pEntitySystem : nullptr;
		if (system)
		{
			for (const auto &[id, state] : m_isolated)
			{
				if (auto *entity = system->GetEntity(id))
				{
					entity->SetFlags(state.flags);
					entity->SetAIObjectID(state.ai_object_id);
					reinterpret_cast<CEntity *>(entity)->EnablePhysics(
					    (state.flags & entity_flag_calc_physics) != 0);
					entity->Hide(state.hidden);
					entity->Activate(state.active);
				}
			}
		}
		m_isolated.clear();
	}

	void native_entity_backend::ensure_sink_registered(
	    Offsets::IEntitySystem &system)
	{
		if (m_sink_system == &system)
			return;
		if (m_sink_system)
			m_sink_system->RemoveSink(&m_sink);
		m_sink_system = &system;
		constexpr std::uint32_t spawn_remove_reuse_subscriptions =
		    (1U << 1) | (1U << 2) | (1U << 3);
		system.AddSink(
		    &m_sink,
		    spawn_remove_reuse_subscriptions,
		    0);
	}

	void native_entity_backend::isolate_entity(Offsets::IEntity *entity)
	{
		if (!m_isolation_active || !entity || m_player_spawn_depth != 0)
			return;
		const auto local = player();
		const auto id = entity->GetId();
		if (entity == local.entity || m_player_entities.contains(id)
		    || m_isolated.contains(id))
			return;
		m_isolated.emplace(
		    id,
		    entity_state{
		        entity->GetFlags(),
		        entity->GetAIObjectID(),
		        entity->IsActive(),
		        entity->IsHidden()});
		auto flags = entity->GetFlags();
		flags &= ~(entity_flag_calc_physics | entity_flag_has_ai
		    | entity_flag_trigger_areas);
		flags |= entity_flag_no_save | entity_flag_clientside_state
		    | entity_flag_no_proximity;
		entity->SetFlags(flags);
		entity->SetAIObjectID(0);
		reinterpret_cast<CEntity *>(entity)->EnablePhysics(false);
		entity->Activate(false);
		entity->Hide(true);
	}

	void native_entity_backend::entity_removed(Offsets::IEntity *entity)
	{
		if (!entity)
			return;
		const auto id = entity->GetId();
		m_isolated.erase(id);
		m_player_entities.erase(id);
	}
}
