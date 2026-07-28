#pragma once

#include "multiplayer/protocol.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace kcd2mp
{
	using remote_avatar_handle = std::uintptr_t;

	struct remote_avatar_snapshot
	{
		player_id id{};
		std::string display_name;
		bool connected{};
		bool has_transform{};
		protocol::TransformState transform;
		protocol::MovementMode movement_mode{
		    protocol::MOVEMENT_MODE_IDLE};
		bool has_avatar{};
		protocol::AvatarDescriptor avatar;
	};

	struct remote_avatar_sync_result
	{
		bool success{true};
		std::string error;
		std::size_t spawned{};
		std::size_t updated{};
		std::size_t removed{};
	};

	class remote_avatar_backend
	{
	public:
		virtual ~remote_avatar_backend() = default;
		[[nodiscard]] virtual bool available() const = 0;
		[[nodiscard]] virtual std::string diagnostic() const = 0;
		[[nodiscard]] virtual std::optional<remote_avatar_handle> spawn(
		    const remote_avatar_snapshot &player) = 0;
		[[nodiscard]] virtual bool update(
		    remote_avatar_handle avatar,
		    const remote_avatar_snapshot &player,
		    bool appearance_changed) = 0;
		virtual void remove(remote_avatar_handle avatar) = 0;
	};

	class remote_avatar_manager
	{
	public:
		explicit remote_avatar_manager(remote_avatar_backend &backend) :
		    m_backend(backend)
		{
		}

		[[nodiscard]] remote_avatar_sync_result sync(
		    std::span<const remote_avatar_snapshot> players)
		{
			remote_avatar_sync_result result;
			const bool needs_avatar = std::ranges::any_of(
			    players,
			    [](const remote_avatar_snapshot &player)
			    {
				    return player.id != 0 && player.has_transform;
			    });
			if (needs_avatar && !m_backend.available())
			{
				result.success = false;
				result.error = m_backend.diagnostic();
				return result;
			}

			std::unordered_set<player_id> present;
			present.reserve(players.size());
			for (const auto &player : players)
			{
				if (player.id == 0)
				{
					continue;
				}
				present.insert(player.id);
				if (!player.has_transform)
				{
					continue;
				}
				if (!player.has_avatar)
				{
					result.success = false;
					result.error =
					    "remote player has no avatar descriptor";
					return result;
				}
				const auto iterator = m_avatars.find(player.id);
				if (iterator == m_avatars.end())
				{
					const auto avatar = m_backend.spawn(player);
					if (!avatar)
					{
						result.success = false;
						result.error =
						    "native remote-player avatar spawn failed";
						return result;
					}
					m_avatars.emplace(
					    player.id,
					    avatar_entry{
					        *avatar,
					        player.avatar.archetype_id(),
					        player.avatar.revision()});
					++result.spawned;
				}
				else if (iterator->second.archetype_id
				    != player.avatar.archetype_id())
				{
					const auto replacement = m_backend.spawn(player);
					if (!replacement)
					{
						result.success = false;
						result.error =
						    "native remote-player avatar replacement failed";
						return result;
					}
					m_backend.remove(iterator->second.handle);
					iterator->second = {
					    *replacement,
					    player.avatar.archetype_id(),
					    player.avatar.revision()};
					++result.spawned;
					++result.removed;
				}
				else if (!m_backend.update(
				             iterator->second.handle,
				             player,
				             iterator->second.avatar_revision
				                 != player.avatar.revision()))
				{
					result.success = false;
					result.error =
					    "native remote-player avatar update failed";
					return result;
				}
				else
				{
					iterator->second.avatar_revision =
					    player.avatar.revision();
					++result.updated;
				}
			}

			for (auto iterator = m_avatars.begin();
			     iterator != m_avatars.end();)
			{
				if (present.contains(iterator->first))
				{
					++iterator;
					continue;
				}
				m_backend.remove(iterator->second.handle);
				iterator = m_avatars.erase(iterator);
				++result.removed;
			}
			return result;
		}

		std::size_t clear()
		{
			const auto count = m_avatars.size();
			for (const auto &[id, avatar] : m_avatars)
			{
				(void)id;
				m_backend.remove(avatar.handle);
			}
			m_avatars.clear();
			return count;
		}

		[[nodiscard]] std::size_t size() const
		{
			return m_avatars.size();
		}

	private:
		struct avatar_entry
		{
			remote_avatar_handle handle{};
			std::string archetype_id;
			std::uint64_t avatar_revision{};
		};

		remote_avatar_backend &m_backend;
		std::unordered_map<player_id, avatar_entry> m_avatars;
	};
}
