#include "kcse/native_runtime.hpp"
#include "multiplayer/profile_reconciler.hpp"

#include <REL/Module.h>
#include <crysystem/CCryAction.h>
#include <crysystem/SSystemGlobalEnvironment.h>
#include <game/S_GameContext.h>
#include <Offsets/vtables/ICVar.h>

#include <chrono>
#include <cmath>
#include <string_view>

namespace kcd2mp::kcse
{
	namespace
	{
		bool normalize(protocol::Quaternion *rotation)
		{
			const auto length_squared =
			    rotation->x() * rotation->x()
			    + rotation->y() * rotation->y()
			    + rotation->z() * rotation->z()
			    + rotation->w() * rotation->w();
			if (!std::isfinite(length_squared)
			    || length_squared < 0.000001F)
				return false;
			const auto inverse = 1.0F / std::sqrt(length_squared);
			rotation->set_x(rotation->x() * inverse);
			rotation->set_y(rotation->y() * inverse);
			rotation->set_z(rotation->z() * inverse);
			rotation->set_w(rotation->w() * inverse);
			return true;
		}

		protocol::Quaternion quaternion_from_matrix(const Matrix34 &matrix)
		{
			protocol::Quaternion result;
			const auto trace = matrix.m00 + matrix.m11 + matrix.m22;
			if (trace > 0.0F)
			{
				const auto scale = std::sqrt(trace + 1.0F) * 2.0F;
				result.set_w(0.25F * scale);
				result.set_x((matrix.m21 - matrix.m12) / scale);
				result.set_y((matrix.m02 - matrix.m20) / scale);
				result.set_z((matrix.m10 - matrix.m01) / scale);
			}
			else if (matrix.m00 > matrix.m11 && matrix.m00 > matrix.m22)
			{
				const auto scale =
				    std::sqrt(1.0F + matrix.m00 - matrix.m11 - matrix.m22)
				    * 2.0F;
				result.set_w((matrix.m21 - matrix.m12) / scale);
				result.set_x(0.25F * scale);
				result.set_y((matrix.m01 + matrix.m10) / scale);
				result.set_z((matrix.m02 + matrix.m20) / scale);
			}
			else if (matrix.m11 > matrix.m22)
			{
				const auto scale =
				    std::sqrt(1.0F + matrix.m11 - matrix.m00 - matrix.m22)
				    * 2.0F;
				result.set_w((matrix.m02 - matrix.m20) / scale);
				result.set_x((matrix.m01 + matrix.m10) / scale);
				result.set_y(0.25F * scale);
				result.set_z((matrix.m12 + matrix.m21) / scale);
			}
			else
			{
				const auto scale =
				    std::sqrt(1.0F + matrix.m22 - matrix.m00 - matrix.m11)
				    * 2.0F;
				result.set_w((matrix.m10 - matrix.m01) / scale);
				result.set_x((matrix.m02 + matrix.m20) / scale);
				result.set_y((matrix.m12 + matrix.m21) / scale);
				result.set_z(0.25F * scale);
			}
			if (!normalize(&result))
			{
				result.Clear();
				result.set_w(1.0F);
			}
			return result;
		}

		std::uint64_t now_ms()
		{
			return static_cast<std::uint64_t>(
			    std::chrono::duration_cast<std::chrono::milliseconds>(
			        std::chrono::steady_clock::now().time_since_epoch())
			        .count());
		}

		std::string address_library_key()
		{
			auto &module = REL::Module::get();
			const auto build = module.build_code();
			if (!build.empty())
				return std::string(build);
			const auto release = module.release();
			return release.empty() ? std::string{"unknown"} :
			                         std::string(release);
		}
	}

	native_runtime::native_runtime(const KCSE::IKCSEInterface &kcse) :
	    m_kcse(kcse),
	    m_address_library(address_library_key()),
	    m_profiles(m_entities),
	    m_remote_backend(m_entities),
	    m_remote_avatars(m_remote_backend)
	{
		std::scoped_lock lock(m_cache_mutex);
		m_capabilities = runtime_capability_kcse;
		m_diagnostic =
		    "Waiting for KCSE PostUpdate and a fully loaded native save.";
	}

	void native_runtime::on_lifecycle(std::uint32_t message_type) noexcept
	{
		switch (message_type)
		{
		case KCSE::IMessagingInterface::kMessage_DataLoaded:
			m_data_loaded.store(true, std::memory_order_release);
			m_epoch_invalidated.store(true, std::memory_order_release);
			break;
		case KCSE::IMessagingInterface::kMessage_PreDataLoaded:
			m_data_loaded.store(false, std::memory_order_release);
			m_epoch_invalidated.store(true, std::memory_order_release);
			break;
		case KCSE::IMessagingInterface::kMessage_LoadGame:
		case KCSE::IMessagingInterface::kMessage_NewGame:
			m_epoch_invalidated.store(true, std::memory_order_release);
			break;
		default:
			break;
		}
	}

	bool native_runtime::on_frame()
	{
		m_frame_seen.store(true, std::memory_order_release);
		const auto changed =
		    m_epoch_invalidated.exchange(false, std::memory_order_acq_rel);
		if (changed)
			invalidate_epoch_on_game_thread();
		refresh_cached_state();
		finish_native_unload_if_complete();
		return changed;
	}

	runtime_descriptor native_runtime::descriptor() const
	{
		std::scoped_lock lock(m_cache_mutex);
		return {
		    m_capabilities,
		    m_kcse.GetKCSEVersion(),
		    m_kcse.GetGameVersion(),
		    m_kcse.GetReleaseIndex(),
		    m_epoch.load(std::memory_order_acquire),
		    m_address_library};
	}

	runtime_gate native_runtime::capability() const
	{
		std::scoped_lock lock(m_cache_mutex);
		if (!m_multiplayer_requested.load(std::memory_order_acquire))
		{
			return {
			    false,
			    false,
			    "Multiplayer runtime is idle; click Connect to initialize it."};
		}
		const auto missing =
		    required_client_runtime_capabilities & ~m_capabilities;
		if (missing == 0)
			return {true, false, {}};
		return {false, !m_probe_failed, m_diagnostic};
	}

	bool native_runtime::can_start_join() const
	{
		std::scoped_lock lock(m_cache_mutex);
		return m_data_loaded.load(std::memory_order_acquire)
		    && m_frame_seen.load(std::memory_order_acquire)
		    && m_local_transform.has_value()
		    && !m_level_id.empty();
	}

	bool native_runtime::prepare_multiplayer()
	{
		if (!can_start_join())
		{
			std::scoped_lock lock(m_cache_mutex);
			m_diagnostic =
			    "Load a native save and wait for the local player before "
			    "connecting.";
			return false;
		}
		m_multiplayer_requested.store(true, std::memory_order_release);
		{
			std::scoped_lock lock(m_cache_mutex);
			m_diagnostic =
			    "Initializing native multiplayer runtime capabilities.";
		}
		return true;
	}

	void native_runtime::cancel_multiplayer_preparation()
	{
		if (!m_multiplayer_requested.exchange(false, std::memory_order_acq_rel)
		    && !m_preparation_active)
			return;
		m_remote_avatars.clear();
		m_remote_backend.clear();
		m_remote_backend.reset_active_probe();
		m_profiles.reset();
		m_preparation_active = false;
		m_preparation_frames = 0;
		m_probe_transform_verified = false;
		m_probe_complete = false;
		m_probe_failed = false;
		m_probe_error.clear();
		std::scoped_lock lock(m_cache_mutex);
		if (!m_sandbox_active && !m_unload_pending)
		{
			m_sandbox_progress = {};
			m_diagnostic =
			    "Multiplayer runtime is idle; click Connect to initialize it.";
		}
	}

	sandbox_start_result native_runtime::begin_sandbox(
	    const protocol::ServerBootstrap &bootstrap)
	{
		std::unique_lock lock(m_cache_mutex);
		if (!m_local_transform)
			return {false, "Native player is not ready."};
		if (bootstrap.level_id().empty() || bootstrap.level_id() != m_level_id)
			return {false, "Loaded native save does not match the server level."};
		if ((m_capabilities & runtime_capability_profile_apply) == 0
		    || !bootstrap.has_profile())
		{
			return {
			    false,
			    "Native profile application or authoritative server profile "
			    "is unavailable."};
		}
		auto *framework = CCryAction::GetInstance();
		if (!framework)
			return {false, "CCryAction is unavailable."};
		framework->AllowSave(false);
		framework->AllowLoad(false);
		m_save_load_locked = true;
		const auto spawn = bootstrap.profile().transform_valid()
		    ? bootstrap.profile().last_transform()
		    : bootstrap.spawn();
		auto target = bootstrap.profile();
		target.set_transform_valid(true);
		*target.mutable_last_transform() = spawn;
		m_profiles.set_wire_identity(target);
		const auto applied = reconcile_profile(m_profiles, target);
		if (!applied.success)
		{
			m_profiles.reset();
			if (!applied.rollback_succeeded)
			{
				lock.unlock();
				begin_native_unload(
				    "Native profile rollback failed; unloading the modified "
				    "save.");
			}
			else
			{
				restore_save_load();
			}
			return {
			    false,
			    "Native profile transaction failed: " + applied.error};
		}
		m_sandbox_active = true;
		m_sandbox_progress.phase = sandbox_phase::ready;
		m_sandbox_progress.initial_spawn = spawn;
		return {true, {}};
	}

	sandbox_poll_result native_runtime::poll_sandbox()
	{
		std::scoped_lock lock(m_cache_mutex);
		return m_sandbox_progress;
	}

	bool native_runtime::sandbox_active() const
	{
		std::scoped_lock lock(m_cache_mutex);
		return m_sandbox_active;
	}

	void native_runtime::end_sandbox()
	{
		{
			std::scoped_lock lock(m_cache_mutex);
			if (!m_sandbox_active || m_unload_pending)
				return;
			m_sandbox_progress.phase = sandbox_phase::unloading;
			m_sandbox_progress.error.clear();
		}
		m_remote_avatars.clear();
		m_remote_backend.clear();
		m_entities.restore_world();
		m_profiles.reset();
		begin_native_unload("Native sandbox world unload is in progress.");
	}

	std::string native_runtime::current_level_id() const
	{
		std::scoped_lock lock(m_cache_mutex);
		return m_level_id;
	}

	std::optional<protocol::PlayerProfile> native_runtime::local_profile()
	{
		std::string error;
		auto result = m_profiles.capture(error);
		if (!result)
		{
			std::scoped_lock lock(m_cache_mutex);
			m_diagnostic = error.empty()
			    ? "Native profile capture failed."
			    : std::move(error);
		}
		return result;
	}

	bool native_runtime::set_non_player_entities_disabled(bool disabled)
	{
		std::string error;
		const auto result =
		    m_entities.set_world_isolated(disabled, error);
		if (!result)
		{
			std::scoped_lock lock(m_cache_mutex);
			m_diagnostic = std::move(error);
		}
		return result;
	}

	std::optional<protocol::TransformState>
	native_runtime::local_transform() const
	{
		std::scoped_lock lock(m_cache_mutex);
		return m_local_transform;
	}

	std::optional<protocol::AvatarDescriptor>
	native_runtime::local_avatar_visual() const
	{
		std::string error;
		auto profile =
		    const_cast<native_profile_backend &>(m_profiles).capture(error);
		return profile
		    ? std::optional<protocol::AvatarDescriptor>(profile->avatar())
		    : std::nullopt;
	}

	bool native_runtime::apply_local_correction(
	    const protocol::TransformState &transform)
	{
		std::string error;
		if (!m_entities.write_transform(
		        m_entities.player().entity,
		        transform,
		        error))
		{
			std::scoped_lock lock(m_cache_mutex);
			m_diagnostic = std::move(error);
			return false;
		}
		std::scoped_lock lock(m_cache_mutex);
		m_local_transform = transform;
		m_transform_sequence = 0;
		return true;
	}

	remote_avatar_sync_result native_runtime::sync_remote_players(
	    std::span<const remote_avatar_snapshot> players)
	{
		m_remote_backend.set_epoch(
		    m_epoch.load(std::memory_order_acquire));
		return m_remote_avatars.sync(players);
	}

	std::uint64_t native_runtime::epoch() const noexcept
	{
		return m_epoch.load(std::memory_order_acquire);
	}

	void native_runtime::invalidate_epoch_on_game_thread()
	{
		m_multiplayer_requested.store(false, std::memory_order_release);
		m_remote_avatars.clear();
		m_remote_backend.clear();
		m_entities.restore_world();
		m_profiles.reset();
		m_remote_backend.reset_active_probe();
		if (!m_unload_pending)
			restore_save_load();
		m_epoch.fetch_add(1, std::memory_order_acq_rel);
		std::scoped_lock lock(m_cache_mutex);
		m_level_id.clear();
		m_local_transform.reset();
		m_transform_sequence = 0;
		m_probe_transform_verified = false;
		m_probe_complete = false;
		m_probe_failed = false;
		m_preparation_active = false;
		m_preparation_frames = 0;
		m_probe_error.clear();
		if (!m_unload_pending)
		{
			m_sandbox_active = false;
			m_sandbox_progress = {};
		}
	}

	void native_runtime::refresh_cached_state()
	{
		const auto multiplayer_requested =
		    m_multiplayer_requested.load(std::memory_order_acquire);
		std::uint64_t capabilities =
		    runtime_capability_kcse | runtime_capability_game_thread;
		std::string level;
		std::optional<protocol::TransformState> transform;

		auto *framework = CCryAction::GetInstance();
		const auto native_player = m_entities.player();
		auto *entity = native_player.entity;
		auto *framework_actor =
		    framework ? framework->GetClientActor() : nullptr;
		auto *context_actor = native_player.actor;
		if (entity && framework_actor && context_actor)
		{
			capabilities |= runtime_capability_local_player;
			if (auto *matrix = entity->GetWorldTMPtr())
			{
				protocol::TransformState state;
				state.mutable_position()->set_x(matrix->m03);
				state.mutable_position()->set_y(matrix->m13);
				state.mutable_position()->set_z(matrix->m23);
				*state.mutable_rotation() = quaternion_from_matrix(*matrix);
				state.mutable_velocity();
				state.set_client_time_ms(now_ms());
				transform = std::move(state);
				capabilities |= runtime_capability_transform_read;
			}
		}

		std::string profile_error;
		const auto profile_ready = multiplayer_requested
		    && (capabilities & runtime_capability_local_player) != 0
		    && m_profiles.ready(profile_error);
		const auto avatar_ready = multiplayer_requested
		    && (capabilities & runtime_capability_local_player) != 0
		    && m_remote_backend.available();
		if (multiplayer_requested && !m_preparation_active)
		{
			m_remote_backend.reset_active_probe();
			m_probe_transform_verified = false;
			m_probe_complete = false;
			m_probe_failed = false;
			m_probe_error.clear();
			m_preparation_frames = 0;
			m_preparation_active = true;
		}
		if (multiplayer_requested && transform && profile_ready && avatar_ready
		    && !m_probe_complete && !m_probe_failed)
		{
			if (!m_probe_transform_verified)
			{
				std::string probe_error;
				if (!m_entities.write_transform(
				        entity,
				        *transform,
				        probe_error))
				{
					m_probe_failed = true;
					m_probe_error =
					    "active identical SetWorldTM probe failed: "
					    + probe_error;
				}
				else
				{
					m_probe_transform_verified = true;
					m_transform_sequence = 0;
				}
			}
			if (m_probe_transform_verified && !m_probe_failed)
			{
				std::string probe_error;
				switch (m_remote_backend.poll_active_probe(
				    *transform,
				    probe_error))
				{
				case native_remote_avatar_backend::active_probe_result::
				    succeeded:
					m_probe_complete = true;
					break;
				case native_remote_avatar_backend::active_probe_result::
				    failed:
					m_probe_failed = true;
					m_probe_error =
					    "active native Actor/Soul/Inventory/Equipment "
					    "probe failed: "
					    + probe_error;
					break;
				case native_remote_avatar_backend::active_probe_result::
				    pending:
					break;
				}
			}
		}
		if (multiplayer_requested && m_preparation_active
		    && !m_probe_complete && !m_probe_failed
		    && ++m_preparation_frames > 900)
		{
			m_probe_failed = true;
			if (!transform)
				m_probe_error =
				    "Native local-player transform was not ready in time.";
			else if (!profile_ready)
				m_probe_error = profile_error.empty()
				    ? "Native player profile was not ready in time."
				    : std::move(profile_error);
			else if (!avatar_ready)
				m_probe_error = m_remote_backend.diagnostic();
			else
				m_probe_error =
				    "Native multiplayer capability probe timed out.";
		}
		if (multiplayer_requested && m_probe_complete)
		{
			capabilities |= runtime_capability_transform_write
			    | runtime_capability_sandbox
			    | runtime_capability_entity_isolation
			    | runtime_capability_equipment
			    | runtime_capability_profile_capture
			    | runtime_capability_profile_apply
			    | runtime_capability_remote_avatar;
		}

		auto *environment = SSystemGlobalEnvironment::GetInstance();
		if (entity && environment && environment->pConsole)
		{
			if (auto *level_cvar =
			        environment->pConsole->GetCVar("wh_sys_BaseLevelId"))
			{
				if (const auto *value = level_cvar->GetString())
					level = value;
			}
		}

		std::scoped_lock lock(m_cache_mutex);
		if (transform)
			transform->set_sequence(++m_transform_sequence);
		m_local_transform = std::move(transform);
		m_level_id = std::move(level);
		m_capabilities = capabilities;
		if ((capabilities & runtime_capability_local_player) == 0)
		{
			m_diagnostic =
			    "Load a native save; CryAction/GameContext has no client "
			    "Actor yet.";
		}
		else if (!multiplayer_requested)
		{
			m_diagnostic =
			    "Multiplayer runtime is idle; click Connect to initialize it.";
		}
		else if ((capabilities & runtime_capability_transform_read) == 0)
		{
			m_diagnostic =
			    "The native player transform is not readable through the "
			    "verified libKCD2 vtable.";
		}
		else if (m_probe_failed)
		{
			m_diagnostic = m_probe_error;
		}
		else if (!m_probe_complete)
		{
			m_diagnostic =
			    "Active native multiplayer ABI probe is running.";
		}
		else
		{
			m_diagnostic =
			    profile_error.empty()
			        ? m_remote_backend.diagnostic()
			        : std::move(profile_error);
			if ((capabilities & runtime_capability_remote_avatar) != 0)
				m_diagnostic =
				    "All native multiplayer runtime capabilities are ready.";
		}
	}

	void native_runtime::restore_save_load()
	{
		if (!m_save_load_locked)
			return;
		if (auto *framework = CCryAction::GetInstance())
		{
			framework->AllowSave(true);
			framework->AllowLoad(true);
		}
		m_save_load_locked = false;
	}

	void native_runtime::begin_native_unload(std::string_view reason)
	{
		auto *framework = CCryAction::GetInstance();
		{
			std::scoped_lock lock(m_cache_mutex);
			m_unload_pending = true;
			m_sandbox_active = true;
			m_sandbox_progress.phase = sandbox_phase::unloading;
			m_sandbox_progress.error = std::string(reason);
		}
		if (framework && framework->m_pGameContext)
			framework->EndGameContext();
		finish_native_unload_if_complete();
	}

	bool native_runtime::native_world_unloaded() const
	{
		auto *framework = CCryAction::GetInstance();
		return !framework
		    || (!framework->m_pGameContext
		        && framework->GetClientEntity() == nullptr);
	}

	void native_runtime::finish_native_unload_if_complete()
	{
		{
			std::scoped_lock lock(m_cache_mutex);
			if (!m_unload_pending)
				return;
		}
		if (!native_world_unloaded())
			return;
		restore_save_load();
		std::scoped_lock lock(m_cache_mutex);
		m_unload_pending = false;
		m_sandbox_active = false;
		m_sandbox_progress = {};
	}
}
