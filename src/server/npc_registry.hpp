#pragma once

#include "multiplayer/protocol.hpp"

#include <chrono>
#include <filesystem>
#include <optional>
#include <span>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace kcd2mp::server
{
	using npc_time_point = std::chrono::steady_clock::time_point;

	class npc_registry
	{
	public:
		struct player_position
		{
			player_id id{};
			const protocol::TransformState *transform{};
			bool connected{};
		};

		enum class event_kind
		{
			enter,
			leave,
			authority
		};

		struct event
		{
			event_kind kind{};
			player_id recipient{};
			protocol::NpcState state;
		};

		npc_registry() = default;
		npc_registry(
		    std::string level_id,
		    const std::filesystem::path &catalog_path);

		void observe(
		    player_id reporter,
		    const protocol::ClientNpcDiscovery &message,
		    const protocol::TransformState *reporter_transform,
		    bool humans_enabled,
		    bool animals_enabled,
		    npc_time_point now);
		[[nodiscard]] bool update(
		    player_id reporter,
		    const protocol::ClientNpcUpdate &message,
		    npc_time_point now);
		[[nodiscard]] std::vector<event> reconcile(
		    std::span<const player_position> players,
		    npc_time_point now);
		[[nodiscard]] std::vector<event> remove_player(
		    player_id id,
		    std::span<const player_position> players,
		    npc_time_point now);
		[[nodiscard]] std::vector<event> disable_kind(
		    protocol::NpcKind kind);
		[[nodiscard]] std::vector<protocol::NpcState> states_for(
		    player_id id) const;
		[[nodiscard]] std::size_t size() const noexcept;

	private:
		struct entry
		{
			protocol::NpcState state;
			npc_time_point lease_expires{};
			npc_time_point last_update{};
			bool observed{};
		};

		[[nodiscard]] bool catalog_allows(
		    std::uint64_t guid,
		    protocol::NpcKind kind) const;
		[[nodiscard]] static float distance_squared(
		    const protocol::TransformState &left,
		    const protocol::TransformState &right);
		[[nodiscard]] std::vector<event> assign_authorities(
		    std::span<const player_position> players,
		    npc_time_point now);

		std::unordered_map<std::uint64_t, entry> m_entries;
		std::unordered_map<player_id, std::unordered_set<std::uint64_t>>
		    m_interest;
		std::unordered_map<std::uint64_t, protocol::NpcKind> m_catalog;
		// A runtime GUID is the discovery token. Mapping it globally prevents
		// two clients observing the same uncatalogued actor from creating two
		// server NPCs at the same position.
		std::unordered_map<std::uint64_t, std::uint64_t> m_dynamic_ids;
		std::uint64_t m_next_lease_id{};
		std::uint64_t m_next_dynamic_id{0x8000000000000000ULL};
		bool m_catalog_required{};
	};
}
