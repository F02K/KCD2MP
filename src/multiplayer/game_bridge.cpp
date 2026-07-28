#include "multiplayer/game_bridge.hpp"

#include "kcd2_init.hpp"
#include "multiplayer/client.hpp"
#include "multiplayer/entity_control.hpp"
#include "multiplayer/remote_avatar.hpp"
#include "npc/catalog.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <format>
#include <string_view>

namespace kcd2mp::game
{
	namespace
	{
		using clock = std::chrono::steady_clock;

		std::uint64_t sequence{};
		big::Vec3 previous_position{};
		clock::time_point previous_sample{};
		bool previous_position_valid{};

		class retail_entity_control_backend final :
		    public entity_control_backend
		{
		public:
			bool is_active(controlled_entity entity) const override
			{
				return static_cast<big::CEntity *>(entity)->IsActive();
			}

			bool is_hidden(controlled_entity entity) const override
			{
				return static_cast<big::CEntity *>(entity)->IsHidden();
			}

			bool set_active(
			    controlled_entity entity,
			    bool active) override
			{
				static_cast<big::CEntity *>(entity)->Activate(active);
				return true;
			}

			bool set_hidden(
			    controlled_entity entity,
			    bool hidden) override
			{
				static_cast<big::CEntity *>(entity)->Hide(hidden);
				return true;
			}
		};

		retail_entity_control_backend entity_backend;
		entity_controller entities{entity_backend};

		class retail_npc_backend final : public npc::backend
		{
		public:
			npc::capability get_capability() const override
			{
				return {
				    false,
				    "the supported retail build has no audited native "
				    "human NPC spawn/animation/equipment/remove path yet"};
			}

			std::optional<npc::native_handle> spawn(
			    const npc::spawn_request &,
			    std::string &) override
			{
				return std::nullopt;
			}

			npc::status poll(npc::native_handle) override
			{
				return {
				    npc::state::failed,
				    npc::error_code::unavailable,
				    get_capability().diagnostic};
			}

			bool set_transform(
			    npc::native_handle,
			    const npc::transform &) override
			{
				return false;
			}

			bool set_locomotion(
			    npc::native_handle,
			    npc::locomotion) override
			{
				return false;
			}

			bool set_appearance(
			    npc::native_handle,
			    const npc::appearance &) override
			{
				return false;
			}

			void remove(npc::native_handle) override
			{
			}
		};

		retail_npc_backend native_npc_backend;
		npc::manager controlled_npcs{native_npc_backend};

		std::uint64_t pack_npc_handle(npc::handle value)
		{
			return static_cast<std::uint64_t>(value.generation) << 32
			    | value.slot;
		}

		npc::handle unpack_npc_handle(std::uint64_t value)
		{
			return {
			    static_cast<std::uint32_t>(value),
			    static_cast<std::uint32_t>(value >> 32)};
		}

		npc::transform npc_transform(
		    const protocol::TransformState &value)
		{
			return {
			    {value.position().x(),
			     value.position().y(),
			     value.position().z()},
			    {value.rotation().x(),
			     value.rotation().y(),
			     value.rotation().z(),
			     value.rotation().w()}};
		}

		npc::appearance npc_appearance(
		    const protocol::AvatarDescriptor &value)
		{
			npc::appearance result;
			result.items.reserve(value.equipment_size());
			for (const auto &item : value.equipment())
			{
				result.items.push_back(
				    {item.definition_id(), item.equipped_slot()});
			}
			result.pose = value.stance() == protocol::AVATAR_STANCE_READY
			    ? npc::stance::ready
			    : npc::stance::relaxed;
			result.weapon = static_cast<npc::weapon_class>(
			    static_cast<int>(value.weapon_class()));
			result.weapon_drawn = value.weapon_drawn();
			return result;
		}

		npc::locomotion npc_locomotion(
		    protocol::MovementMode value)
		{
			switch (value)
			{
			case protocol::MOVEMENT_MODE_WALK:
				return npc::locomotion::walk;
			case protocol::MOVEMENT_MODE_RUN:
				return npc::locomotion::run;
			default:
				return npc::locomotion::idle;
			}
		}

		class retail_remote_avatar_backend final :
		    public remote_avatar_backend
		{
		public:
			bool available() const override
			{
				return controlled_npcs.get_capability().available;
			}

			std::string diagnostic() const override
			{
				return controlled_npcs.get_capability().diagnostic;
			}

			std::optional<remote_avatar_handle> spawn(
			    const remote_avatar_snapshot &player) override
			{
				npc::spawn_request request;
				request.archetype_id = player.avatar.archetype_id();
				request.world_transform =
				    npc_transform(player.transform);
				request.movement =
				    npc_locomotion(player.movement_mode);
				request.visual = npc_appearance(player.avatar);
				request.exempt_from_entity_control = true;
				const auto spawned = controlled_npcs.spawn(
				    std::move(request),
				    player.id);
				return spawned
				    ? std::optional<remote_avatar_handle>{
				          pack_npc_handle(spawned.npc)}
				    : std::nullopt;
			}

			bool update(
			    remote_avatar_handle avatar,
			    const remote_avatar_snapshot &player,
			    bool appearance_changed) override
			{
				const auto handle = unpack_npc_handle(avatar);
				const auto status = controlled_npcs.get_status(handle);
				if (status.value == npc::state::failed
				    || status.value == npc::state::removed)
				{
					return false;
				}
				const bool dynamic_applied = controlled_npcs.set_transform(
				           handle,
				           npc_transform(player.transform))
				    && controlled_npcs.set_locomotion(
				        handle,
				        npc_locomotion(player.movement_mode));
				return dynamic_applied
				    && (!appearance_changed
				        || controlled_npcs.set_appearance(
				            handle,
				            npc_appearance(player.avatar)));
			}

			void remove(remote_avatar_handle avatar) override
			{
				(void)controlled_npcs.remove(
				    unpack_npc_handle(avatar));
			}
		};

		retail_remote_avatar_backend remote_avatar_backend;
		remote_avatar_manager remote_avatars{remote_avatar_backend};

		struct cvar_change
		{
			std::string_view name;
			int previous{};
		};

		struct sandbox_runtime
		{
			sandbox_phase phase{sandbox_phase::idle};
			std::string expected_level_id;
			std::string map_name;
			std::string error;
			std::optional<protocol::TransformState> initial_spawn;
			std::optional<protocol::ServerBootstrap> bootstrap;
			std::vector<cvar_change> cvar_changes;
			clock::time_point deadline;
			std::optional<clock::time_point> player_ready_since;
			bool player_observed{};
		};

		sandbox_runtime sandbox;

		constexpr std::array level_names{
		    std::pair{"0", "rataje"},
		    std::pair{"1", "rataje_dlc4"},
		    std::pair{"2", "trosecko"},
		    std::pair{"3", "kutnohorsko"},
		    std::pair{"4", "klaster"},
		    std::pair{"256", "test_switching256"},
		    std::pair{"257", "test_switching257"},
		    std::pair{"258", "concept_level_switch_1"},
		    std::pair{"259", "concept_level_switch_2"},
		    std::pair{"300", "empty"},
		    std::pair{"400", "test_save"},
		    std::pair{"401", "test_switch_first"},
		    std::pair{"402", "test_switch_second"},
		    std::pair{"500", "player_switching"},
		    std::pair{"501", "player_switching2"},
		};

		constexpr std::array required_cvars{
		    std::pair{"g_EnableLoadSave", 0},
		    std::pair{"autotest_disable_saveload", 1},
		    // The retail map path identifies a frontend map load as a new game
		    // and otherwise plays intro_new_game. This Warhorse-owned switch is
		    // registered without VF_CHEAT in the supported retail image.
		    std::pair{"wh_sys_HideIntroVideo", 1},
		    // Do not disable all movie sequences here: the native level-start
		    // sequence also dismisses the loading UI and activates the player.
		    std::pair{"g_disableSequencePlayback", 0},
		    std::pair{"wh_sys_FreezePlayline", 1},
		    std::pair{"wh_sys_NoPlaylineDeleting", 1},
		    std::pair{"wh_sys_AutoLoadLastSave", 0},
		    std::pair{"g_asynclevelload", 0},
		};

		std::optional<std::string_view> map_name_for_level(
		    std::string_view level_id)
		{
			const auto found = std::ranges::find(
			    level_names,
			    level_id,
			    &decltype(level_names)::value_type::first);
			return found == level_names.end()
			    ? std::nullopt
			    : std::optional<std::string_view>(found->second);
		}

		void restore_cvars()
		{
			for (auto change = sandbox.cvar_changes.rbegin();
			     change != sandbox.cvar_changes.rend();
			     ++change)
			{
				if (!big::engine_cvar_set_int(
				        change->name,
				        change->previous))
				{
					LOGF(
					    ERROR,
					    "Sandbox cleanup could not restore {}={}.",
					    change->name,
					    change->previous);
				}
			}
			sandbox.cvar_changes.clear();
		}

		void reset_sandbox_runtime()
		{
			sandbox = {};
			previous_position_valid = false;
		}

		bool apply_sandbox_cvars(std::string &error)
		{
			for (const auto &[name, value] : required_cvars)
			{
				const auto previous = big::engine_cvar_int(name);
				if (!previous)
				{
					error = std::format(
					    "required sandbox CVar '{}' is unavailable",
					    name);
					restore_cvars();
					return false;
				}
				sandbox.cvar_changes.push_back({name, *previous});
				if (!big::engine_cvar_set_int(name, value))
				{
					error = std::format(
					    "sandbox CVar '{}' rejected value {}",
					    name,
					    value);
					restore_cvars();
					return false;
				}
			}
			return true;
		}

		protocol::Quaternion quaternion_from_matrix(const float (&matrix)[12])
		{
			protocol::Quaternion result;
			const auto trace = matrix[0] + matrix[5] + matrix[10];
			if (trace > 0.0F)
			{
				const auto scale = std::sqrt(trace + 1.0F) * 2.0F;
				result.set_w(0.25F * scale);
				result.set_x((matrix[9] - matrix[6]) / scale);
				result.set_y((matrix[2] - matrix[8]) / scale);
				result.set_z((matrix[4] - matrix[1]) / scale);
			}
			else if (matrix[0] > matrix[5] && matrix[0] > matrix[10])
			{
				const auto scale =
				    std::sqrt(1.0F + matrix[0] - matrix[5] - matrix[10]) * 2.0F;
				result.set_w((matrix[9] - matrix[6]) / scale);
				result.set_x(0.25F * scale);
				result.set_y((matrix[1] + matrix[4]) / scale);
				result.set_z((matrix[2] + matrix[8]) / scale);
			}
			else if (matrix[5] > matrix[10])
			{
				const auto scale =
				    std::sqrt(1.0F + matrix[5] - matrix[0] - matrix[10]) * 2.0F;
				result.set_w((matrix[2] - matrix[8]) / scale);
				result.set_x((matrix[1] + matrix[4]) / scale);
				result.set_y(0.25F * scale);
				result.set_z((matrix[6] + matrix[9]) / scale);
			}
			else
			{
				const auto scale =
				    std::sqrt(1.0F + matrix[10] - matrix[0] - matrix[5]) * 2.0F;
				result.set_w((matrix[4] - matrix[1]) / scale);
				result.set_x((matrix[2] + matrix[8]) / scale);
				result.set_y((matrix[6] + matrix[9]) / scale);
				result.set_z(0.25F * scale);
			}
			(void)normalize_rotation(&result);
			return result;
		}

		void matrix_from_transform(
		    const protocol::TransformState &transform,
		    float (&matrix)[12])
		{
			auto rotation = transform.rotation();
			if (!normalize_rotation(&rotation))
			{
				rotation.Clear();
				rotation.set_w(1.0F);
			}
			const auto x = rotation.x();
			const auto y = rotation.y();
			const auto z = rotation.z();
			const auto w = rotation.w();

			matrix[0] = 1.0F - 2.0F * (y * y + z * z);
			matrix[1] = 2.0F * (x * y - z * w);
			matrix[2] = 2.0F * (x * z + y * w);
			matrix[3] = transform.position().x();
			matrix[4] = 2.0F * (x * y + z * w);
			matrix[5] = 1.0F - 2.0F * (x * x + z * z);
			matrix[6] = 2.0F * (y * z - x * w);
			matrix[7] = transform.position().y();
			matrix[8] = 2.0F * (x * z - y * w);
			matrix[9] = 2.0F * (y * z + x * w);
			matrix[10] = 1.0F - 2.0F * (x * x + y * y);
			matrix[11] = transform.position().z();
		}
	}

	sandbox_gate sandbox_capability()
	{
		if (!big::engine_console_available())
		{
			return {false, "Sandbox engine console is not initialized yet."};
		}
		for (const auto command : {"unload"})
		{
			if (!big::engine_console_has_command(command))
			{
				return {
				    false,
				    std::format(
				        "Required retail console command '{}' is unavailable.",
				        command)};
			}
		}
		for (const auto &[name, value] : required_cvars)
		{
			(void)value;
			if (!big::engine_cvar_available(name))
			{
				return {
				    false,
				    std::format(
				        "Required sandbox CVar '{}' is unavailable.",
				        name)};
			}
		}
		if (!big::engine_cvar_available("wh_sys_BaseLevelId"))
		{
			return {
			    false,
			    "Required level-detection CVar 'wh_sys_BaseLevelId' is unavailable."};
		}
		if (!big::g_CEntity_SetWorldTM)
		{
			return {
			    false,
			    "The signature-gated player transform wrapper is unavailable."};
		}
		return {
		    true,
		    "Retail sandbox wrappers are ready; joining will adopt a natively loaded save."};
	}

	bool can_start_join()
	{
		return sandbox.phase == sandbox_phase::idle
		    && big::g_player_entity
		    && !current_level_id().empty();
	}

	sandbox_start_result begin_sandbox(
	    const protocol::ServerBootstrap &bootstrap)
	{
		const auto capability = sandbox_capability();
		if (!capability.available)
		{
			return {false, capability.diagnostic};
		}
		if (sandbox.phase != sandbox_phase::idle)
		{
			return {false, "another sandbox transition is already active"};
		}
		if (!big::g_player_entity)
		{
			return {
			    false,
			    "the native save was no longer loaded when the server bootstrap arrived"};
		}
		const auto map_name = map_name_for_level(bootstrap.level_id());
		if (!map_name)
		{
			return {
			    false,
			    std::format(
			        "server requested unsupported retail level id '{}'",
			        bootstrap.level_id())};
		}
		const auto loaded_level = current_level_id();
		if (loaded_level.empty())
		{
			return {
			    false,
			    "the loaded save has no detectable retail level id"};
		}
		if (loaded_level != bootstrap.level_id())
		{
			return {
			    false,
			    std::format(
			        "loaded save is on level id {}, but the server requires level id {}",
			        loaded_level,
			        bootstrap.level_id())};
		}

		sandbox.bootstrap = bootstrap;
		sandbox.expected_level_id = bootstrap.level_id();
		sandbox.map_name = *map_name;
		sandbox.error.clear();
		sandbox.initial_spawn.reset();
		sandbox.player_observed = false;
		if (!apply_sandbox_cvars(sandbox.error))
		{
			const auto error = sandbox.error;
			reset_sandbox_runtime();
			return {false, error};
		}

		sandbox.player_observed = true;
		sandbox.player_ready_since = clock::now();
		LOGF(
		    INFO,
		    "Sandbox bootstrap adopted the natively loaded save on server level {}; save/load is now locked.",
		    sandbox.expected_level_id);
		sandbox.phase = sandbox_phase::loading;
		const auto timeout = std::max<std::uint32_t>(
		    5,
		    bootstrap.timeout_seconds() > 5
		        ? bootstrap.timeout_seconds() - 5
		        : bootstrap.timeout_seconds());
		sandbox.deadline = clock::now() + std::chrono::seconds(timeout);
		return {true, {}};
	}

	sandbox_poll_result poll_sandbox()
	{
		if (sandbox.phase == sandbox_phase::unloading)
		{
			sandbox.player_observed =
			    sandbox.player_observed || big::g_player_entity != nullptr;
			if (sandbox.player_observed && !big::g_player_entity)
			{
				restore_cvars();
				LOG(INFO) << "Sandbox unloaded and engine CVars restored.";
				reset_sandbox_runtime();
			}
			else if (clock::now() >= sandbox.deadline)
			{
				sandbox.phase = sandbox_phase::failed;
				sandbox.error =
				    "sandbox unload timed out; save/load remains locked for safety";
				LOG(ERROR) << sandbox.error;
			}
			return {sandbox.phase, sandbox.error, std::nullopt};
		}

		if (sandbox.phase != sandbox_phase::loading)
		{
			return {
			    sandbox.phase,
			    sandbox.error,
			    sandbox.initial_spawn};
		}
		if (clock::now() >= sandbox.deadline)
		{
			sandbox.phase = sandbox_phase::failed;
			sandbox.error = std::format(
			    "timed out loading retail level '{}' (id {})",
			    sandbox.map_name,
			    sandbox.expected_level_id);
			return {sandbox.phase, sandbox.error, std::nullopt};
		}

		const auto level = current_level_id();
		if (!big::g_player_entity || level.empty())
		{
			sandbox.player_ready_since.reset();
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		if (level != sandbox.expected_level_id)
		{
			sandbox.player_ready_since.reset();
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		sandbox.player_observed = true;
		if (!sandbox.player_ready_since)
		{
			sandbox.player_ready_since = clock::now();
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		// The Dude entity is published before the retail map transition has
		// completely settled. Requiring a stable player/level pair avoids
		// sending ClientWorldReady while the native loading UI is still closing.
		if (clock::now() - *sandbox.player_ready_since
		    < std::chrono::seconds(5))
		{
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		if (!sandbox.bootstrap)
		{
			sandbox.phase = sandbox_phase::failed;
			sandbox.error = "sandbox bootstrap state was lost while loading";
			return {sandbox.phase, sandbox.error, std::nullopt};
		}

		const auto &bootstrap = *sandbox.bootstrap;
		const protocol::TransformState *target_spawn = nullptr;
		if (bootstrap.has_profile()
		    && bootstrap.profile().transform_valid()
		    && bootstrap.profile().has_last_transform())
		{
			target_spawn = &bootstrap.profile().last_transform();
		}
		else if (bootstrap.spawn_valid() && bootstrap.has_spawn())
		{
			target_spawn = &bootstrap.spawn();
		}
		if (target_spawn)
		{
			if (!is_finite_transform(*target_spawn))
			{
				sandbox.phase = sandbox_phase::failed;
				sandbox.error = "server supplied an invalid sandbox spawn transform";
				return {sandbox.phase, sandbox.error, std::nullopt};
			}
			apply_local_correction(*target_spawn);
		}

		const auto spawned = local_transform();
		if (!spawned)
		{
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		if (bootstrap.mode() == protocol::BOOTSTRAP_MODE_INITIALIZE)
		{
			sandbox.initial_spawn = *spawned;
		}
		sandbox.phase = sandbox_phase::ready;
		LOGF(
		    INFO,
		    "Sandbox level {} is ready and the server spawn was applied.",
		    sandbox.expected_level_id);
		return {
		    sandbox.phase,
		    {},
		    sandbox.initial_spawn};
	}

	bool sandbox_active()
	{
		return sandbox.phase != sandbox_phase::idle;
	}

	void end_sandbox()
	{
		if (sandbox.phase == sandbox_phase::idle
		    || sandbox.phase == sandbox_phase::unloading)
		{
			return;
		}
		sandbox.initial_spawn.reset();
		sandbox.bootstrap.reset();
		const auto removed_avatars = remote_avatars.clear();
		if (removed_avatars != 0)
		{
			LOGF(
			    INFO,
			    "Removed {} remote multiplayer avatars.",
			    removed_avatars);
		}
		(void)set_non_player_entities_disabled(false);
		if (!big::g_player_entity)
		{
			restore_cvars();
			reset_sandbox_runtime();
			return;
		}
		if (!big::engine_console_execute("unload", true))
		{
			sandbox.phase = sandbox_phase::failed;
			sandbox.error =
			    "could not queue sandbox unload; save/load remains locked for safety";
			LOG(ERROR) << sandbox.error;
			return;
		}
		sandbox.phase = sandbox_phase::unloading;
		sandbox.deadline = clock::now() + std::chrono::seconds(30);
		LOG(INFO) << "Sandbox unload queued.";
	}

	std::string current_level_id()
	{
		if (!big::g_player_entity)
		{
			return {};
		}
		const auto level = big::engine_cvar_string("wh_sys_BaseLevelId");
		return level.value_or(std::string{});
	}

	std::optional<protocol::TransformState> local_transform()
	{
		if (!big::g_player_entity)
		{
			previous_position_valid = false;
			return std::nullopt;
		}

		float matrix[12];
		big::g_player_entity->GetWorldTM(matrix);
		const big::Vec3 position{matrix[3], matrix[7], matrix[11]};
		const auto now = std::chrono::steady_clock::now();

		big::Vec3 velocity{};
		if (previous_position_valid && now > previous_sample)
		{
			const auto seconds =
			    std::chrono::duration<float>(now - previous_sample).count();
			if (seconds > 0.0F)
			{
				velocity = (position - previous_position) * (1.0F / seconds);
			}
		}
		previous_position = position;
		previous_sample = now;
		previous_position_valid = true;

		protocol::TransformState result;
		result.mutable_position()->set_x(position.x);
		result.mutable_position()->set_y(position.y);
		result.mutable_position()->set_z(position.z);
		*result.mutable_rotation() = quaternion_from_matrix(matrix);
		result.mutable_velocity()->set_x(velocity.x);
		result.mutable_velocity()->set_y(velocity.y);
		result.mutable_velocity()->set_z(velocity.z);
		result.set_sequence(++sequence);
		result.set_client_time_ms(
		    static_cast<std::uint64_t>(
		        std::chrono::duration_cast<std::chrono::milliseconds>(
		            now.time_since_epoch())
		            .count()));
		return result;
	}

	std::optional<protocol::PlayerProfile> local_profile()
	{
		// Profile capture is part of the native sandbox risk gate. Returning no
		// snapshot keeps the client from claiming it captured authoritative RPG
		// or inventory data through an unaudited fallback.
		return std::nullopt;
	}

	void apply_local_correction(const protocol::TransformState &transform)
	{
		if (!big::g_player_entity || !is_finite_transform(transform))
		{
			return;
		}
		float matrix[12];
		matrix_from_transform(transform, matrix);
		big::g_player_entity->SetWorldTM(matrix);
	}

	bool set_non_player_entities_disabled(bool disabled)
	{
		if (big::g_player_entity)
		{
			(void)entities.register_player(big::g_player_entity);
		}
		std::vector<controlled_entity> current;
		current.reserve(big::g_entities.size());
		for (auto *entity : big::g_entities)
		{
			current.push_back(entity);
		}
		const auto result = entities.set_disabled(disabled, current);
		LOGF(
		    result.failed == 0 ? INFO : ERROR,
		    "Server entity control {}: affected={}, restored={}, failed={}.",
		    disabled ? "disabled non-player entities"
		             : "restored non-player entities",
		    result.affected,
		    result.restored,
		    result.failed);
		return result.failed == 0;
	}

	bool non_player_entities_disabled()
	{
		return entities.disabled();
	}

	void register_player_entity(void *entity)
	{
		const auto result = entities.register_player(entity);
		if (result.failed != 0)
		{
			LOG(ERROR)
			    << "Could not restore a newly registered multiplayer player entity.";
			if (g_multiplayer_client)
			{
				g_multiplayer_client->fail(
				    "could not exempt a multiplayer player entity "
				    "from server entity control");
			}
		}
	}

	void unregister_player_entity(void *entity)
	{
		entities.unregister_player(entity);
	}

	void on_entity_created(void *entity)
	{
		const auto result = entities.entity_created(entity);
		if (result.failed != 0)
		{
			LOG(ERROR)
			    << "Could not apply server entity control to a newly created entity.";
			if (g_multiplayer_client)
			{
				g_multiplayer_client->fail(
				    "could not apply server entity control "
				    "to a newly created entity");
			}
		}
	}

	void on_entity_destroyed(void *entity)
	{
		entities.entity_destroyed(entity);
		controlled_npcs.native_destroyed(
		    reinterpret_cast<npc::native_handle>(entity));
	}

	npc::manager &npc_manager()
	{
		return controlled_npcs;
	}

	void game_thread_tick()
	{
		static const bool catalog_loaded = []
		{
			std::string error;
			if (!npc::initialize_runtime_catalog(error))
			{
				LOG(ERROR) << "NPC catalog initialization failed: " << error;
				return false;
			}
			LOGF(
			    INFO,
			    "Loaded {} human Soul archetypes from local Tables.pak.",
			    npc::runtime_catalog().size());
			return true;
		}();
		(void)catalog_loaded;
		if (!g_multiplayer_client)
		{
			return;
		}
		(void)poll_sandbox();
		controlled_npcs.tick();
		const auto level = current_level_id();
		g_multiplayer_client->game_tick(
		    local_transform(),
		    std::nullopt,
		    level);
		const auto status = g_multiplayer_client->status();
		if (status.state == client_state::connected
		    || status.state == client_state::reconnecting)
		{
			std::vector<remote_avatar_snapshot> snapshots;
			for (const auto &player :
			     g_multiplayer_client->remote_players())
			{
				snapshots.push_back({
				    player.id,
				    player.display_name,
				    player.connected,
				    player.has_transform,
				    player.transform,
				    player.movement_mode,
				    player.has_avatar,
				    player.avatar});
			}
			const auto avatars = remote_avatars.sync(snapshots);
			if (!avatars.success)
			{
				g_multiplayer_client->fail(avatars.error);
			}
		}
		else if (remote_avatars.size() != 0)
		{
			(void)remote_avatars.clear();
		}
		if (const auto correction = g_multiplayer_client->take_local_correction())
		{
			apply_local_correction(*correction);
		}
		if (g_multiplayer_client->status().state == client_state::disconnected
		    && sandbox_active())
		{
			end_sandbox();
		}
	}
}
