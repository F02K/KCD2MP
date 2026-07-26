#include "server/world_store.hpp"

#include <toml++/toml.hpp>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <bcrypt.h>

namespace kcd2mp::server
{
	namespace
	{
		constexpr std::uint32_t manifest_schema = 1;

		std::string toml_escape(std::string_view value)
		{
			std::string result;
			result.reserve(value.size());
			for (const char character : value)
			{
				if (character == '\\' || character == '"')
				{
					result.push_back('\\');
				}
				result.push_back(character);
			}
			return result;
		}

		void atomic_replace(
		    const std::filesystem::path &target,
		    std::span<const std::byte> bytes)
		{
			const auto temporary = target.wstring() + L".tmp";
			{
				std::ofstream output(
				    std::filesystem::path(temporary),
				    std::ios::binary | std::ios::trunc);
				if (!output
				    || !output.write(
				        reinterpret_cast<const char *>(bytes.data()),
				        static_cast<std::streamsize>(bytes.size())))
				{
					throw std::runtime_error(
					    "could not write persistent file: " + target.string());
				}
				output.flush();
				if (!output)
				{
					throw std::runtime_error(
					    "could not flush persistent file: " + target.string());
				}
			}
			if (!MoveFileExW(
			        temporary.c_str(),
			        target.c_str(),
			        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
			{
				DeleteFileW(temporary.c_str());
				throw std::runtime_error(
				    "could not atomically replace persistent file: "
				    + target.string());
			}
		}

		void atomic_replace(
		    const std::filesystem::path &target,
		    std::string_view text)
		{
			atomic_replace(
			    target,
			    {reinterpret_cast<const std::byte *>(text.data()), text.size()});
		}

		protocol::TransformState configured_spawn(
		    const initial_spawn_config &source)
		{
			protocol::TransformState result;
			result.mutable_position()->set_x(source.x);
			result.mutable_position()->set_y(source.y);
			result.mutable_position()->set_z(source.z);
			result.mutable_rotation()->set_x(source.qx);
			result.mutable_rotation()->set_y(source.qy);
			result.mutable_rotation()->set_z(source.qz);
			result.mutable_rotation()->set_w(source.qw);
			result.mutable_velocity();
			return result;
		}

		std::uint64_t random_u64()
		{
			std::uint64_t value{};
			if (BCryptGenRandom(
			        nullptr,
			        reinterpret_cast<PUCHAR>(&value),
			        sizeof(value),
			        BCRYPT_USE_SYSTEM_PREFERRED_RNG)
			    < 0)
			{
				throw std::runtime_error("BCryptGenRandom failed");
			}
			// TOML integer values are signed 64-bit. Keep the generated seed in
			// that portable range while preserving 63 bits of entropy.
			value &= 0x7FFFFFFFFFFFFFFFULL;
			return value == 0 ? 1 : value;
		}
	}

	std::string random_hex(std::size_t byte_count)
	{
		if (byte_count == 0 || byte_count > 1024)
		{
			throw std::invalid_argument("random byte count is invalid");
		}
		std::vector<std::uint8_t> bytes(byte_count);
		if (BCryptGenRandom(
		        nullptr,
		        bytes.data(),
		        static_cast<ULONG>(bytes.size()),
		        BCRYPT_USE_SYSTEM_PREFERRED_RNG)
		    < 0)
		{
			throw std::runtime_error("BCryptGenRandom failed");
		}
		std::ostringstream stream;
		stream << std::hex << std::setfill('0');
		for (const auto byte : bytes)
		{
			stream << std::setw(2) << static_cast<unsigned int>(byte);
		}
		return stream.str();
	}

	token_hash hash_token(std::string_view token)
	{
		BCRYPT_ALG_HANDLE algorithm{};
		BCRYPT_HASH_HANDLE hash{};
		token_hash result{};
		if (BCryptOpenAlgorithmProvider(
		        &algorithm,
		        BCRYPT_SHA256_ALGORITHM,
		        nullptr,
		        0)
		    < 0)
		{
			throw std::runtime_error("BCryptOpenAlgorithmProvider failed");
		}
		const auto close_algorithm = [&]
		{
			BCryptCloseAlgorithmProvider(algorithm, 0);
		};
		if (BCryptCreateHash(algorithm, &hash, nullptr, 0, nullptr, 0, 0) < 0)
		{
			close_algorithm();
			throw std::runtime_error("BCryptCreateHash failed");
		}
		const auto status = BCryptHashData(
		    hash,
		    reinterpret_cast<PUCHAR>(const_cast<char *>(token.data())),
		    static_cast<ULONG>(token.size()),
		    0);
		const auto finish = status < 0
		    ? status
		    : BCryptFinishHash(
		        hash,
		        reinterpret_cast<PUCHAR>(result.data()),
		        static_cast<ULONG>(result.size()),
		        0);
		BCryptDestroyHash(hash);
		close_algorithm();
		if (finish < 0)
		{
			throw std::runtime_error("BCrypt token hashing failed");
		}
		return result;
	}

	bool secure_equal(
	    std::span<const std::byte> left,
	    std::span<const std::byte> right)
	{
		if (left.size() != right.size())
		{
			return false;
		}
		unsigned char difference{};
		for (std::size_t index = 0; index < left.size(); ++index)
		{
			difference |= std::to_integer<unsigned char>(
			    left[index] ^ right[index]);
		}
		return difference == 0;
	}

	world_store::world_store(const server_config &config) :
	    m_root(config.world_directory),
	    m_profiles_directory(m_root / "players")
	{
		std::filesystem::create_directories(m_profiles_directory);
		load_or_create(config);
		load_profiles();
	}

	const session_manifest &world_store::manifest() const
	{
		return m_manifest;
	}

	std::vector<persisted_profile> world_store::profiles() const
	{
		return m_profiles;
	}

	std::optional<persisted_profile> world_store::find_by_token(
	    std::string_view token) const
	{
		const auto candidate = hash_token(token);
		const auto iterator = std::ranges::find_if(
		    m_profiles,
		    [&](const persisted_profile &profile)
		    {
			    return secure_equal(candidate, profile.identity_hash);
		    });
		return iterator == m_profiles.end()
		    ? std::nullopt
		    : std::optional{*iterator};
	}

	std::optional<persisted_profile> world_store::find_by_player_id(
	    player_id id) const
	{
		const auto iterator = std::ranges::find_if(
		    m_profiles,
		    [id](const persisted_profile &profile)
		    {
			    return profile.profile.player_id() == id;
		    });
		return iterator == m_profiles.end()
		    ? std::nullopt
		    : std::optional{*iterator};
	}

	player_id world_store::allocate_player_id()
	{
		const auto result = m_manifest.next_player_id++;
		write_manifest();
		return result;
	}

	void world_store::set_spawn(protocol::TransformState spawn)
	{
		if (!is_finite_transform(spawn)
		    || !normalize_rotation(spawn.mutable_rotation()))
		{
			throw std::invalid_argument("session spawn is invalid");
		}
		m_manifest.spawn = std::move(spawn);
		m_manifest.spawn_valid = true;
		++m_manifest.revision;
		write_manifest();
	}

	void world_store::save_profile(
	    const token_hash &identity_hash,
	    const protocol::PlayerProfile &profile)
	{
		if (!is_valid_profile(profile) || profile.player_id() == 0)
		{
			throw std::invalid_argument("persistent player profile is invalid");
		}
		auto iterator = std::ranges::find_if(
		    m_profiles,
		    [&](const persisted_profile &stored)
		    {
			    return stored.profile.player_id() == profile.player_id();
		    });
		if (iterator == m_profiles.end())
		{
			m_profiles.push_back({identity_hash, profile});
			iterator = std::prev(m_profiles.end());
		}
		else
		{
			iterator->identity_hash = identity_hash;
			iterator->profile = profile;
		}
		write_profile(*iterator);
	}

	void world_store::load_or_create(const server_config &config)
	{
		const auto path = m_root / "session.toml";
		if (!std::filesystem::exists(path))
		{
			m_manifest.server_id = random_hex(16);
			m_manifest.session_id = random_hex(16);
			m_manifest.world_seed = random_u64();
			m_manifest.level_id = config.level_id;
			m_manifest.content_hash = config.required_content_hash;
			if (config.initial_spawn)
			{
				m_manifest.spawn = configured_spawn(*config.initial_spawn);
				m_manifest.spawn_valid = true;
			}
			write_manifest();
			return;
		}

		const auto document = toml::parse_file(path.string());
		const auto *session = document["session"].as_table();
		if (!session
		    || (*session)["schema"].value_or(0U) != manifest_schema)
		{
			throw std::runtime_error("world/session.toml has an unsupported schema");
		}
		m_manifest.server_id = (*session)["server_id"].value_or(std::string{});
		m_manifest.session_id = (*session)["session_id"].value_or(std::string{});
		m_manifest.revision = (*session)["revision"].value_or<std::uint64_t>(1);
		m_manifest.world_seed =
		    (*session)["world_seed"].value_or<std::uint64_t>(0);
		m_manifest.level_id = (*session)["level_id"].value_or(std::string{});
		m_manifest.content_hash =
		    (*session)["content_hash"].value_or(std::string{});
		m_manifest.sandbox_mode =
		    (*session)["sandbox_mode"].value_or(std::string{});
		m_manifest.spawn_valid = (*session)["spawn_valid"].value_or(false);
		m_manifest.next_player_id =
		    (*session)["next_player_id"].value_or<std::uint64_t>(1);
		if (m_manifest.server_id.empty() || m_manifest.session_id.empty()
		    || m_manifest.level_id != config.level_id
		    || m_manifest.content_hash != config.required_content_hash
		    || m_manifest.sandbox_mode != "isolated_multiplayer"
		    || m_manifest.next_player_id == 0)
		{
			throw std::runtime_error(
			    "world/session.toml does not match server.toml");
		}
		if (m_manifest.spawn_valid)
		{
			const auto *spawn = document["spawn"].as_table();
			if (!spawn)
			{
				throw std::runtime_error("session spawn is missing");
			}
			auto *position = m_manifest.spawn.mutable_position();
			auto *rotation = m_manifest.spawn.mutable_rotation();
			m_manifest.spawn.mutable_velocity();
			position->set_x((*spawn)["x"].value_or(
			    std::numeric_limits<float>::quiet_NaN()));
			position->set_y((*spawn)["y"].value_or(
			    std::numeric_limits<float>::quiet_NaN()));
			position->set_z((*spawn)["z"].value_or(
			    std::numeric_limits<float>::quiet_NaN()));
			rotation->set_x((*spawn)["qx"].value_or(
			    std::numeric_limits<float>::quiet_NaN()));
			rotation->set_y((*spawn)["qy"].value_or(
			    std::numeric_limits<float>::quiet_NaN()));
			rotation->set_z((*spawn)["qz"].value_or(
			    std::numeric_limits<float>::quiet_NaN()));
			rotation->set_w((*spawn)["qw"].value_or(
			    std::numeric_limits<float>::quiet_NaN()));
			if (!is_finite_transform(m_manifest.spawn)
			    || !normalize_rotation(m_manifest.spawn.mutable_rotation()))
			{
				throw std::runtime_error("session spawn is invalid");
			}
		}
	}

	void world_store::load_profiles()
	{
		for (const auto &entry :
		     std::filesystem::directory_iterator(m_profiles_directory))
		{
			if (!entry.is_regular_file() || entry.path().extension() != ".pb")
			{
				continue;
			}
			std::ifstream input(entry.path(), std::ios::binary);
			const std::string bytes{
			    std::istreambuf_iterator<char>(input),
			    std::istreambuf_iterator<char>()};
			protocol::StoredPlayerProfile stored;
			if ((!input && !input.eof()) || !stored.ParseFromString(bytes)
			    || stored.identity_token_hash().size() != 32
			    || !stored.has_profile() || !is_valid_profile(stored.profile())
			    || stored.profile().player_id() == 0)
			{
				throw std::runtime_error(
				    "invalid persistent profile: " + entry.path().string());
			}
			persisted_profile profile;
			std::ranges::copy(
			    stored.identity_token_hash(),
			    reinterpret_cast<char *>(profile.identity_hash.data()));
			profile.profile = std::move(*stored.mutable_profile());
			m_profiles.push_back(std::move(profile));
		}
	}

	void world_store::write_manifest() const
	{
		std::ostringstream output;
		output << "[session]\n"
		       << "schema = " << manifest_schema << '\n'
		       << "server_id = \"" << toml_escape(m_manifest.server_id) << "\"\n"
		       << "session_id = \"" << toml_escape(m_manifest.session_id) << "\"\n"
		       << "revision = " << m_manifest.revision << '\n'
		       << "world_seed = " << m_manifest.world_seed << '\n'
		       << "level_id = \"" << toml_escape(m_manifest.level_id) << "\"\n"
		       << "content_hash = \"" << toml_escape(m_manifest.content_hash)
		       << "\"\n"
		       << "sandbox_mode = \""
		       << toml_escape(m_manifest.sandbox_mode) << "\"\n"
		       << "spawn_valid = " << (m_manifest.spawn_valid ? "true" : "false")
		       << '\n'
		       << "next_player_id = " << m_manifest.next_player_id << '\n';
		if (m_manifest.spawn_valid)
		{
			output << "\n[spawn]\n"
			       << std::setprecision(9)
			       << "x = " << m_manifest.spawn.position().x() << '\n'
			       << "y = " << m_manifest.spawn.position().y() << '\n'
			       << "z = " << m_manifest.spawn.position().z() << '\n'
			       << "qx = " << m_manifest.spawn.rotation().x() << '\n'
			       << "qy = " << m_manifest.spawn.rotation().y() << '\n'
			       << "qz = " << m_manifest.spawn.rotation().z() << '\n'
			       << "qw = " << m_manifest.spawn.rotation().w() << '\n';
		}
		atomic_replace(m_root / "session.toml", output.str());
	}

	void world_store::write_profile(const persisted_profile &profile) const
	{
		protocol::StoredPlayerProfile stored;
		stored.set_identity_token_hash(
		    profile.identity_hash.data(),
		    profile.identity_hash.size());
		*stored.mutable_profile() = profile.profile;
		std::string bytes;
		if (!stored.SerializeToString(&bytes))
		{
			throw std::runtime_error("could not serialize persistent profile");
		}
		atomic_replace(
		    m_profiles_directory
		        / (std::to_string(profile.profile.player_id()) + ".pb"),
		    {reinterpret_cast<const std::byte *>(bytes.data()), bytes.size()});
	}
}
