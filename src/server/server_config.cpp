#include "server/server_config.hpp"

#include <toml++/toml.hpp>

#include <cmath>
#include <limits>
#include <stdexcept>

namespace kcd2mp::server
{
	namespace
	{
		template<typename Target>
		Target checked_integer(
		    const toml::table &table,
		    std::string_view key,
		    Target fallback)
		{
			const auto raw = table[key].value<std::int64_t>();
			if (!raw)
			{
				return fallback;
			}
			if (*raw < 0
			    || static_cast<std::uint64_t>(*raw)
			        > static_cast<std::uint64_t>(std::numeric_limits<Target>::max()))
			{
				throw std::runtime_error("server.toml value is out of range: "
				    + std::string(key));
			}
			return static_cast<Target>(*raw);
		}
	}

	server_config load_server_config(const std::filesystem::path &path)
	{
		const auto document = toml::parse_file(path.string());
		const auto *server = document["server"].as_table();
		if (!server)
		{
			throw std::runtime_error("server.toml is missing the [server] table");
		}

		server_config config;
		config.bind_address = (*server)["bind_address"].value_or(config.bind_address);
		config.port = checked_integer(*server, "port", config.port);
		config.name = (*server)["name"].value_or(config.name);
		config.password = (*server)["password"].value_or(std::string{});
		config.max_players = checked_integer(*server, "max_players", config.max_players);
		config.level_id = (*server)["level_id"].value_or(std::string{});
		config.required_content_hash =
		    (*server)["required_content_hash"].value_or(std::string{});
		config.tick_rate = checked_integer(*server, "tick_rate", config.tick_rate);
		config.snapshot_rate =
		    checked_integer(*server, "snapshot_rate", config.snapshot_rate);
		config.handshake_timeout_seconds = checked_integer(
		    *server,
		    "handshake_timeout_seconds",
		    config.handshake_timeout_seconds);
		config.idle_timeout_seconds = checked_integer(
		    *server,
		    "idle_timeout_seconds",
		    config.idle_timeout_seconds);
		config.reconnect_grace_seconds = checked_integer(
		    *server,
		    "reconnect_grace_seconds",
		    config.reconnect_grace_seconds);
		config.bootstrap_timeout_seconds = checked_integer(
		    *server,
		    "bootstrap_timeout_seconds",
		    config.bootstrap_timeout_seconds);
		config.profile_snapshot_interval_seconds = checked_integer(
		    *server,
		    "profile_snapshot_interval_seconds",
		    config.profile_snapshot_interval_seconds);
		config.max_player_speed_mps =
		    (*server)["max_player_speed_mps"].value_or(config.max_player_speed_mps);
		config.movement_tolerance_m =
		    (*server)["movement_tolerance_m"].value_or(config.movement_tolerance_m);
		config.world_directory =
		    (*server)["world_directory"].value_or(config.world_directory.string());
		if (config.world_directory.is_relative())
		{
			config.world_directory =
			    std::filesystem::absolute(path).parent_path()
			    / config.world_directory;
		}
		if (const auto *spawn = (*server)["initial_spawn"].as_table())
		{
			const auto x = (*spawn)["x"].value<double>();
			const auto y = (*spawn)["y"].value<double>();
			const auto z = (*spawn)["z"].value<double>();
			const auto qx = (*spawn)["qx"].value<double>();
			const auto qy = (*spawn)["qy"].value<double>();
			const auto qz = (*spawn)["qz"].value<double>();
			const auto qw = (*spawn)["qw"].value<double>();
			if (!x || !y || !z || !qx || !qy || !qz || !qw)
			{
				throw std::runtime_error(
				    "[server.initial_spawn] requires x, y, z, qx, qy, qz, and qw");
			}
			config.initial_spawn = initial_spawn_config{
			    static_cast<float>(*x),
			    static_cast<float>(*y),
			    static_cast<float>(*z),
			    static_cast<float>(*qx),
			    static_cast<float>(*qy),
			    static_cast<float>(*qz),
			    static_cast<float>(*qw)};
		}
		validate_server_config(config);
		return config;
	}

	void validate_server_config(const server_config &config)
	{
		if (config.bind_address.empty())
		{
			throw std::runtime_error("bind_address must not be empty");
		}
		if (config.port == 0)
		{
			throw std::runtime_error("port must be between 1 and 65535");
		}
		if (config.name.empty() || config.name.size() > 64)
		{
			throw std::runtime_error("server name must contain 1 to 64 bytes");
		}
		if (config.max_players == 0 || config.max_players > 8)
		{
			throw std::runtime_error("max_players must be between 1 and 8");
		}
		if (config.level_id.empty() || config.level_id.size() > 128)
		{
			throw std::runtime_error("level_id must contain 1 to 128 bytes");
		}
		if (config.tick_rate < 10 || config.tick_rate > 120)
		{
			throw std::runtime_error("tick_rate must be between 10 and 120");
		}
		if (config.snapshot_rate == 0 || config.snapshot_rate > config.tick_rate)
		{
			throw std::runtime_error(
			    "snapshot_rate must be positive and no greater than tick_rate");
		}
		if (config.handshake_timeout_seconds == 0
		    || config.idle_timeout_seconds == 0
		    || config.reconnect_grace_seconds == 0
		    || config.bootstrap_timeout_seconds < 30
		    || config.bootstrap_timeout_seconds > 600
		    || config.profile_snapshot_interval_seconds < 5
		    || config.profile_snapshot_interval_seconds > 60)
		{
			throw std::runtime_error("timeouts and profile interval are invalid");
		}
		if (!std::isfinite(config.max_player_speed_mps)
		    || config.max_player_speed_mps <= 0.0F
		    || !std::isfinite(config.movement_tolerance_m)
		    || config.movement_tolerance_m < 0.0F)
		{
			throw std::runtime_error("movement limits must be finite and valid");
		}
		if (config.world_directory.empty())
		{
			throw std::runtime_error("world_directory must not be empty");
		}
		if (config.initial_spawn)
		{
			const auto &spawn = *config.initial_spawn;
			const auto finite = std::isfinite(spawn.x) && std::isfinite(spawn.y)
			    && std::isfinite(spawn.z) && std::isfinite(spawn.qx)
			    && std::isfinite(spawn.qy) && std::isfinite(spawn.qz)
			    && std::isfinite(spawn.qw);
			const auto length = std::sqrt(
			    spawn.qx * spawn.qx + spawn.qy * spawn.qy
			    + spawn.qz * spawn.qz + spawn.qw * spawn.qw);
			if (!finite || length < 0.999F || length > 1.001F)
			{
				throw std::runtime_error(
				    "initial_spawn must be finite with a normalized quaternion");
			}
		}
	}
}
