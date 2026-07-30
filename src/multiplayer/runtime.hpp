#pragma once

#include "kcd2mp.pb.h"
#include "multiplayer/runtime_capabilities.hpp"

#include <cstdint>
#include <optional>
#include <string>

namespace kcd2mp
{
	enum class sandbox_phase
	{
		idle,
		loading,
		ready,
		failed,
		unloading
	};

	struct runtime_descriptor
	{
		std::uint64_t capabilities{};
		std::uint32_t kcse_version{};
		std::uint32_t game_version{};
		std::uint32_t release_index{};
		std::uint64_t epoch{};
		std::string address_library;
	};

	struct runtime_gate
	{
		bool available{};
		bool pending{};
		std::string diagnostic;
	};

	struct sandbox_start_result
	{
		bool started{};
		std::string error;
	};

	struct sandbox_poll_result
	{
		sandbox_phase phase{sandbox_phase::idle};
		std::string error;
		std::optional<protocol::TransformState> initial_spawn;
	};

	class client_runtime
	{
	public:
		virtual ~client_runtime() = default;

		[[nodiscard]] virtual runtime_descriptor descriptor() const = 0;
		[[nodiscard]] virtual runtime_gate capability() const = 0;
		[[nodiscard]] virtual bool can_start_join() const = 0;
		[[nodiscard]] virtual bool prepare_multiplayer() = 0;
		virtual void cancel_multiplayer_preparation() = 0;
		[[nodiscard]] virtual sandbox_start_result begin_sandbox(
		    const protocol::ServerBootstrap &bootstrap) = 0;
		[[nodiscard]] virtual sandbox_poll_result poll_sandbox() = 0;
		[[nodiscard]] virtual bool sandbox_active() const = 0;
		virtual void end_sandbox() = 0;
		[[nodiscard]] virtual std::string current_level_id() const = 0;
		[[nodiscard]] virtual std::optional<protocol::PlayerProfile>
		local_profile() = 0;
		[[nodiscard]] virtual bool set_non_player_entities_disabled(
		    bool disabled) = 0;
	};
}
