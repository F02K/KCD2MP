#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include "npc/catalog.hpp"
#include "server/starter_profile.hpp"

namespace kcd2mp::server
{
	struct initial_spawn_config
	{
		float x{};
		float y{};
		float z{};
		float qx{};
		float qy{};
		float qz{};
		float qw{1.0F};
	};

	struct server_config
	{
		std::string bind_address{"0.0.0.0"};
		std::uint16_t port{27020};
		std::string name{"KCD2MP Server"};
		std::string password;
		std::uint32_t max_players{8};
		std::string level_id;
		std::string required_content_hash;
		std::uint32_t tick_rate{30};
		std::uint32_t snapshot_rate{20};
		std::uint32_t handshake_timeout_seconds{10};
		std::uint32_t idle_timeout_seconds{15};
		std::uint32_t reconnect_grace_seconds{30};
		std::uint32_t bootstrap_timeout_seconds{180};
		std::uint32_t profile_snapshot_interval_seconds{15};
		bool disable_non_player_entities{};
		std::string default_avatar_archetype{
		    npc::default_soul_id};
		std::vector<std::string> allowed_avatar_archetypes{
		    std::string(npc::default_soul_id)};
		std::unordered_set<std::string> known_avatar_archetypes{
		    std::string(npc::default_soul_id)};
		float max_player_speed_mps{15.0F};
		float movement_tolerance_m{2.0F};
		std::filesystem::path world_directory{"world"};
		std::filesystem::path starter_profile_path{"starter_profile.toml"};
		starter_profile_template starter_profile{
		    default_starter_profile_template()};
		std::optional<initial_spawn_config> initial_spawn;
	};

	[[nodiscard]] server_config load_server_config(
	    const std::filesystem::path &path);
	void normalize_avatar_config(server_config &config);
	void validate_server_config(const server_config &config);
}
