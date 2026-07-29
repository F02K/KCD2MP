#pragma once

#include "multiplayer/protocol.hpp"
#include "npc/catalog.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <format>
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

	enum class remote_avatar_state
	{
		pending,
		ready,
		failed
	};

	struct remote_avatar_backend_status
	{
		remote_avatar_state state{remote_avatar_state::pending};
		std::string diagnostic;
	};

	struct remote_avatar_sync_result
	{
		bool success{true};
		bool degraded{};
		std::string error;
		std::string diagnostic;
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
		[[nodiscard]] virtual remote_avatar_backend_status status(
		    remote_avatar_handle avatar) const = 0;
		[[nodiscard]] virtual bool update(
		    remote_avatar_handle avatar,
		    const remote_avatar_snapshot &player,
		    bool appearance_changed) = 0;
		virtual void remove(remote_avatar_handle avatar) = 0;
	};

	class remote_avatar_manager
	{
	public:
		using clock = std::chrono::steady_clock;

		explicit remote_avatar_manager(remote_avatar_backend &backend) :
		    m_backend(backend)
		{
		}

		[[nodiscard]] remote_avatar_sync_result sync(
		    std::span<const remote_avatar_snapshot> players,
		    clock::time_point now = clock::now())
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
					continue;
				present.insert(player.id);
				if (!player.has_transform)
					continue;
				if (!player.has_avatar)
				{
					result.success = false;
					result.error =
					    "remote player has no avatar descriptor";
					return result;
				}

				auto iterator = m_avatars.find(player.id);
				if (iterator == m_avatars.end())
				{
					avatar_entry entry;
					if (!spawn_desired_or_fallback(
					        entry,
					        player,
					        now,
					        result))
					{
						return result;
					}
					iterator = m_avatars.emplace(
					    player.id,
					    std::move(entry)).first;
				}
				if (!sync_entry(
				        iterator->second,
				        player,
				        now,
				        result))
				{
					return result;
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
				remove_entry(iterator->second, result);
				iterator = m_avatars.erase(iterator);
			}
			return result;
		}

		std::size_t clear()
		{
			const auto count = m_avatars.size();
			for (auto &[id, avatar] : m_avatars)
			{
				(void)id;
				if (avatar.candidate)
					m_backend.remove(*avatar.candidate);
				if (avatar.active)
					m_backend.remove(avatar.active);
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
			remote_avatar_handle active{};
			bool active_fallback{};
			std::string active_archetype;
			std::uint64_t active_revision{};
			std::optional<remote_avatar_handle> candidate;
			std::string candidate_archetype;
			std::uint64_t candidate_revision{};
			std::size_t retry_attempt{};
			clock::time_point next_retry{};
		};

		[[nodiscard]] static remote_avatar_snapshot fallback_snapshot(
		    const remote_avatar_snapshot &player)
		{
			auto fallback = player;
			fallback.avatar.set_archetype_id(
			    std::string(npc::default_soul_id));
			return fallback;
		}

		static std::chrono::seconds retry_delay(std::size_t attempt)
		{
			if (attempt >= 5)
				return std::chrono::seconds(30);
			return std::chrono::seconds(std::size_t{1} << attempt);
		}

		static void append_diagnostic(
		    remote_avatar_sync_result &result,
		    std::string message)
		{
			result.degraded = true;
			if (!result.diagnostic.empty())
				result.diagnostic += "; ";
			result.diagnostic += std::move(message);
		}

		void schedule_retry(
		    avatar_entry &entry,
		    clock::time_point now)
		{
			entry.next_retry =
			    now + retry_delay(entry.retry_attempt);
			++entry.retry_attempt;
		}

		bool spawn_fallback(
		    avatar_entry &entry,
		    const remote_avatar_snapshot &player,
		    clock::time_point now,
		    remote_avatar_sync_result &result,
		    std::string diagnostic)
		{
			if (player.avatar.archetype_id()
			    == npc::default_soul_id)
			{
				result.success = false;
				result.error = std::format(
				    "player {}: default Soul {} lifecycle failed",
				    player.id,
				    npc::default_soul_id);
				if (!diagnostic.empty())
					result.error += ": " + diagnostic;
				return false;
			}
			const auto fallback = fallback_snapshot(player);
			const auto handle = m_backend.spawn(fallback);
			if (!handle)
			{
				result.success = false;
				result.error = std::format(
				    "player {}: fallback Soul {} lifecycle failed for desired Soul {}",
				    player.id,
				    npc::default_soul_id,
				    player.avatar.archetype_id());
				if (!diagnostic.empty())
					result.error += ": " + diagnostic;
				return false;
			}
			entry.active = *handle;
			entry.active_fallback = true;
			entry.active_archetype = fallback.avatar.archetype_id();
			entry.active_revision = fallback.avatar.revision();
			const auto delay = retry_delay(entry.retry_attempt);
			schedule_retry(entry, now);
			++result.spawned;
			append_diagnostic(
			    result,
			    std::format(
			        "player {}: {}; desired Soul {}, fallback Soul {}; next retry in {}s",
			        player.id,
			        diagnostic.empty()
			            ? "using built-in fallback avatar"
			            : std::move(diagnostic),
			        player.avatar.archetype_id(),
			        npc::default_soul_id,
			        delay.count()));
			return true;
		}

		bool spawn_desired_or_fallback(
		    avatar_entry &entry,
		    const remote_avatar_snapshot &player,
		    clock::time_point now,
		    remote_avatar_sync_result &result)
		{
			if (const auto handle = m_backend.spawn(player))
			{
				entry.active = *handle;
				entry.active_archetype =
				    player.avatar.archetype_id();
				entry.active_revision = player.avatar.revision();
				++result.spawned;
				return true;
			}
			return spawn_fallback(
			    entry,
			    player,
			    now,
			    result,
			    "desired remote-player avatar spawn failed");
		}

		bool handle_active_failure(
		    avatar_entry &entry,
		    const remote_avatar_snapshot &player,
		    clock::time_point now,
		    remote_avatar_sync_result &result,
		    std::string diagnostic)
		{
			if (entry.active)
			{
				m_backend.remove(entry.active);
				entry.active = 0;
				++result.removed;
			}
			if (entry.candidate)
			{
				m_backend.remove(*entry.candidate);
				entry.candidate.reset();
				++result.removed;
			}
			if (entry.active_fallback)
			{
				result.success = false;
				result.error = std::format(
				    "player {}: fallback Soul {} failed for desired Soul {}",
				    player.id,
				    npc::default_soul_id,
				    player.avatar.archetype_id());
				if (!diagnostic.empty())
					result.error += ": " + diagnostic;
				return false;
			}
			return spawn_fallback(
			    entry,
			    player,
			    now,
			    result,
			    diagnostic.empty()
			        ? "desired remote-player avatar failed"
			        : std::move(diagnostic));
		}

		void fail_candidate(
		    avatar_entry &entry,
		    const remote_avatar_snapshot &player,
		    clock::time_point now,
		    remote_avatar_sync_result &result,
		    std::string diagnostic)
		{
			if (entry.candidate)
			{
				m_backend.remove(*entry.candidate);
				entry.candidate.reset();
				++result.removed;
			}
			entry.candidate_archetype.clear();
			entry.candidate_revision = 0;
			const auto delay = retry_delay(entry.retry_attempt);
			schedule_retry(entry, now);
			append_diagnostic(
			    result,
			    std::format(
			        "player {}: desired Soul {} retry failed ({}); fallback Soul {}; next retry in {}s",
			        player.id,
			        player.avatar.archetype_id(),
			        diagnostic.empty() ? "no diagnostic" : std::move(diagnostic),
			        npc::default_soul_id,
			        delay.count()));
		}

		void start_candidate(
		    avatar_entry &entry,
		    const remote_avatar_snapshot &player,
		    clock::time_point now,
		    remote_avatar_sync_result &result)
		{
			const auto candidate = m_backend.spawn(player);
			if (!candidate)
			{
				const auto delay = retry_delay(entry.retry_attempt);
				schedule_retry(entry, now);
				append_diagnostic(
				    result,
				    std::format(
				        "player {}: desired Soul {} retry spawn failed; fallback Soul {}; next retry in {}s",
				        player.id,
				        player.avatar.archetype_id(),
				        npc::default_soul_id,
				        delay.count()));
				return;
			}
			entry.candidate = *candidate;
			entry.candidate_archetype =
			    player.avatar.archetype_id();
			entry.candidate_revision = player.avatar.revision();
			++result.spawned;
		}

		bool sync_candidate(
		    avatar_entry &entry,
		    const remote_avatar_snapshot &player,
		    clock::time_point now,
		    remote_avatar_sync_result &result)
		{
			if (!entry.candidate)
				return true;
			if (entry.candidate_archetype
			    != player.avatar.archetype_id())
			{
				m_backend.remove(*entry.candidate);
				entry.candidate.reset();
				++result.removed;
				start_candidate(entry, player, now, result);
				return true;
			}
			const auto candidate_status =
			    m_backend.status(*entry.candidate);
			if (candidate_status.state == remote_avatar_state::failed)
			{
				fail_candidate(
				    entry,
				    player,
				    now,
				    result,
				    candidate_status.diagnostic);
				return true;
			}
			if (!m_backend.update(
			        *entry.candidate,
			        player,
			        entry.candidate_revision
			            != player.avatar.revision()))
			{
				fail_candidate(
				    entry,
				    player,
				    now,
				    result,
				    "remote-player avatar retry update failed");
				return true;
			}
			entry.candidate_revision = player.avatar.revision();
			++result.updated;
			if (candidate_status.state != remote_avatar_state::ready)
				return true;

			m_backend.remove(entry.active);
			++result.removed;
			entry.active = *entry.candidate;
			entry.active_fallback = false;
			entry.active_archetype = entry.candidate_archetype;
			entry.active_revision = entry.candidate_revision;
			entry.candidate.reset();
			entry.candidate_archetype.clear();
			entry.candidate_revision = 0;
			entry.retry_attempt = 0;
			entry.next_retry = {};
			return true;
		}

		bool sync_entry(
		    avatar_entry &entry,
		    const remote_avatar_snapshot &player,
		    clock::time_point now,
		    remote_avatar_sync_result &result)
		{
			if (entry.active_fallback
			    && player.avatar.archetype_id()
			           == npc::default_soul_id)
			{
				if (entry.candidate)
				{
					m_backend.remove(*entry.candidate);
					entry.candidate.reset();
					entry.candidate_archetype.clear();
					entry.candidate_revision = 0;
					++result.removed;
				}
				entry.active_fallback = false;
				entry.active_archetype =
				    player.avatar.archetype_id();
				entry.retry_attempt = 0;
				entry.next_retry = {};
			}
			const auto active_status = m_backend.status(entry.active);
			if (active_status.state == remote_avatar_state::failed)
			{
				return handle_active_failure(
				    entry,
				    player,
				    now,
				    result,
				    active_status.diagnostic);
			}

			const auto rendered = entry.active_fallback
			    ? fallback_snapshot(player)
			    : player;
			if (!m_backend.update(
			        entry.active,
			        rendered,
			        entry.active_revision
			            != rendered.avatar.revision()))
			{
				return handle_active_failure(
				    entry,
				    player,
				    now,
				    result,
				    "native remote-player avatar update failed");
			}
			entry.active_revision = rendered.avatar.revision();
			++result.updated;

			const bool needs_replacement = entry.active_fallback
			    || entry.active_archetype
			        != player.avatar.archetype_id();
			if (needs_replacement && !entry.candidate
			    && now >= entry.next_retry)
			{
				start_candidate(entry, player, now, result);
			}
			return sync_candidate(entry, player, now, result);
		}

		void remove_entry(
		    avatar_entry &entry,
		    remote_avatar_sync_result &result)
		{
			if (entry.candidate)
			{
				m_backend.remove(*entry.candidate);
				++result.removed;
			}
			if (entry.active)
			{
				m_backend.remove(entry.active);
				++result.removed;
			}
		}

		remote_avatar_backend &m_backend;
		std::unordered_map<player_id, avatar_entry> m_avatars;
	};
}
