#pragma once

#include <cstdint>

namespace kcd2mp
{
	enum runtime_capability : std::uint64_t
	{
		runtime_capability_kcse = 1ULL << 0,
		runtime_capability_game_thread = 1ULL << 1,
		runtime_capability_local_player = 1ULL << 2,
		runtime_capability_transform_read = 1ULL << 3,
		runtime_capability_transform_write = 1ULL << 4,
		runtime_capability_sandbox = 1ULL << 5,
		runtime_capability_entity_isolation = 1ULL << 6,
		runtime_capability_remote_avatar = 1ULL << 7,
		runtime_capability_equipment = 1ULL << 8,
		runtime_capability_profile_capture = 1ULL << 9,
		runtime_capability_profile_apply = 1ULL << 10,
	};

	constexpr std::uint64_t known_client_runtime_capabilities =
	    (runtime_capability_profile_apply << 1) - 1;

	constexpr std::uint64_t required_client_runtime_capabilities =
	    known_client_runtime_capabilities;
}
