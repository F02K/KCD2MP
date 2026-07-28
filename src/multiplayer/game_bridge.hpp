#pragma once

#include "multiplayer/protocol.hpp"
#include "npc/npc.hpp"

#include <cstdint>
#include <optional>
#include <string>

namespace kcd2mp::game
{
	struct entity_handle
	{
		std::uint32_t id{};

		[[nodiscard]] explicit operator bool() const
		{
			return id != 0;
		}

		friend bool operator==(const entity_handle &, const entity_handle &) = default;
	};

	struct sandbox_gate
	{
		bool available{};
		std::string diagnostic;
	};

	struct sandbox_start_result
	{
		bool started{};
		std::string error;
	};

	enum class sandbox_phase
	{
		idle,
		loading,
		ready,
		failed,
		unloading
	};

	struct sandbox_poll_result
	{
		sandbox_phase phase{sandbox_phase::idle};
		std::string error;
		std::optional<protocol::TransformState> initial_spawn;
	};

	[[nodiscard]] sandbox_gate sandbox_capability();
	[[nodiscard]] bool can_start_join();
	[[nodiscard]] sandbox_start_result begin_sandbox(
	    const protocol::ServerBootstrap &bootstrap);
	[[nodiscard]] sandbox_poll_result poll_sandbox();
	[[nodiscard]] bool sandbox_active();
	void end_sandbox();
	[[nodiscard]] std::string current_level_id();
	[[nodiscard]] std::optional<protocol::TransformState> local_transform();
	[[nodiscard]] std::optional<protocol::PlayerProfile> local_profile();
	void apply_local_correction(const protocol::TransformState &transform);
	[[nodiscard]] bool set_non_player_entities_disabled(bool disabled);
	[[nodiscard]] bool non_player_entities_disabled();
	void register_player_entity(void *entity);
	void unregister_player_entity(void *entity);
	void on_entity_created(void *entity);
	void on_entity_destroyed(void *entity);
	[[nodiscard]] npc::manager &npc_manager();
	void game_thread_tick();
}
