#pragma once

#include "kcse/bridge_api.hpp"

#include <string>

namespace kcd2mp::kcse
{
	struct bridge_status
	{
		bool available{};
		std::uint32_t kcse_version{};
		std::uint32_t game_version{};
		std::uint32_t runtime_flags{};
		std::string diagnostic;
	};

	[[nodiscard]] bool refresh_bridge() noexcept;
	[[nodiscard]] bridge_status current_bridge_status();
	[[nodiscard]] bool resolve_entity(std::uint32_t entity_id, entity_view &result) noexcept;
	[[nodiscard]] bool queue_task(task_callback callback, void *context) noexcept;
}
