#include "kcse/remote_avatar_readiness.hpp"
#include "multiplayer/remote_avatar.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <ranges>
#include <unordered_map>
#include <vector>

namespace
{
	class fake_backend final : public kcd2mp::remote_avatar_backend
	{
	public:
		bool available() const override
		{
			return enabled;
		}

		std::string diagnostic() const override
		{
			return "avatar backend unavailable";
		}

		std::optional<kcd2mp::remote_avatar_handle> spawn(
		    const kcd2mp::remote_avatar_snapshot &player) override
		{
			++spawn_attempts;
			if ((!desired_spawns_succeed
			        && player.avatar.archetype_id()
			            != kcd2mp::npc::default_soul_id)
			    || (!fallback_spawns_succeed
			        && player.avatar.archetype_id()
			            == kcd2mp::npc::default_soul_id))
			{
				return std::nullopt;
			}
			const auto handle = next_handle++;
			players[handle] = player;
			states[handle] = spawned_state;
			return handle;
		}

		kcd2mp::remote_avatar_backend_status status(
		    kcd2mp::remote_avatar_handle avatar) const override
		{
			const auto found = states.find(avatar);
			return found == states.end()
			    ? kcd2mp::remote_avatar_backend_status{
			          kcd2mp::remote_avatar_state::failed,
			          "avatar is missing"}
			    : kcd2mp::remote_avatar_backend_status{
			          found->second,
			          found->second == kcd2mp::remote_avatar_state::failed
			              ? "injected failure"
			              : ""};
		}

		bool update(
		    kcd2mp::remote_avatar_handle avatar,
		    const kcd2mp::remote_avatar_snapshot &player,
		    bool appearance_changed) override
		{
			players[avatar] = player;
			if (appearance_changed)
				++appearance_updates;
			return updates_succeed;
		}

		void remove(kcd2mp::remote_avatar_handle avatar) override
		{
			players.erase(avatar);
			states.erase(avatar);
			++removed;
		}

		bool enabled{true};
		bool desired_spawns_succeed{true};
		bool fallback_spawns_succeed{true};
		bool updates_succeed{true};
		kcd2mp::remote_avatar_state spawned_state{
		    kcd2mp::remote_avatar_state::ready};
		kcd2mp::remote_avatar_handle next_handle{1};
		std::size_t removed{};
		std::size_t spawn_attempts{};
		std::size_t appearance_updates{};
		std::unordered_map<
		    kcd2mp::remote_avatar_handle,
		    kcd2mp::remote_avatar_snapshot>
		    players;
		std::unordered_map<
		    kcd2mp::remote_avatar_handle,
		    kcd2mp::remote_avatar_state>
		    states;
	};

	kcd2mp::remote_avatar_snapshot player(
	    kcd2mp::player_id id,
	    float x,
	    kcd2mp::protocol::MovementMode mode)
	{
		kcd2mp::remote_avatar_snapshot result;
		result.id = id;
		result.display_name = "Remote";
		result.connected = true;
		result.has_transform = true;
		result.transform.mutable_position()->set_x(x);
		result.transform.mutable_rotation()->set_w(1.0F);
		result.transform.mutable_velocity();
		result.movement_mode = mode;
		result.has_avatar = true;
		result.avatar.set_archetype_id(
		    "763db0bb-4469-497d-bdc9-712b3df91b5a");
		result.avatar.set_revision(1);
		return result;
	}
}

int main()
{
	using namespace kcd2mp;
	using namespace std::chrono_literals;
	const auto soul_applied_at = std::chrono::steady_clock::now();
	assert(!kcse::evaluate_remote_soul_settle(
	             10,
	             10,
	             soul_applied_at,
	             soul_applied_at)
	             .ready);
	assert(!kcse::evaluate_remote_soul_settle(
	             13,
	             10,
	             soul_applied_at + 249ms,
	             soul_applied_at)
	             .ready);
	assert(!kcse::evaluate_remote_soul_settle(
	             12,
	             10,
	             soul_applied_at + 250ms,
	             soul_applied_at)
	             .ready);
	assert(kcse::evaluate_remote_soul_settle(
	           13,
	           10,
	           soul_applied_at + 250ms,
	           soul_applied_at)
	           .ready);
	fake_backend backend;
	remote_avatar_manager manager(backend);
	std::vector players{
	    player(1, 10.0F, protocol::MOVEMENT_MODE_IDLE),
	    player(2, 20.0F, protocol::MOVEMENT_MODE_WALK)};

	auto result = manager.sync(players);
	assert(result.success);
	assert(result.spawned == 2);
	assert(manager.size() == 2);
	assert(std::ranges::all_of(
	    backend.players,
	    [](const auto &entry)
	    {
		    return entry.second.display_name == "Remote";
	    }));

	players[0].display_name = "Renamed Remote";
	players[0].transform.mutable_position()->set_x(11.0F);
	players[0].movement_mode = protocol::MOVEMENT_MODE_RUN;
	players[0].connected = false;
	players.pop_back();
	result = manager.sync(players);
	assert(result.success);
	assert(result.updated == 1);
	assert(backend.appearance_updates == 0);
	assert(result.removed == 1);
	assert(manager.size() == 1);
	assert(backend.players.begin()->second.display_name
	    == "Renamed Remote");

	players[0].avatar.set_revision(2);
	result = manager.sync(players);
	assert(result.success);
	assert(backend.appearance_updates == 1);

	players[0].avatar.set_archetype_id(
	    "11111111-2222-4333-8444-555555555555");
	result = manager.sync(players);
	assert(result.success);
	assert(result.spawned == 1);
	assert(result.removed == 1);
	assert(manager.size() == 1);

	assert(manager.clear() == 1);
	assert(manager.size() == 0);

	backend.enabled = false;
	result = manager.sync(players);
	assert(!result.success);
	assert(result.error == "avatar backend unavailable");
	assert(manager.size() == 0);

	fake_backend fallback_backend;
	fallback_backend.desired_spawns_succeed = false;
	remote_avatar_manager fallback_manager(fallback_backend);
	auto fallback_players = std::vector{
	    player(3, 30.0F, protocol::MOVEMENT_MODE_IDLE)};
	fallback_players[0].avatar.set_archetype_id(
	    "11111111-2222-4333-8444-555555555555");
	const auto start = remote_avatar_manager::clock::now();
	result = fallback_manager.sync(fallback_players, start);
	assert(result.success);
	assert(result.degraded);
	assert(fallback_manager.size() == 1);
	assert(fallback_backend.players.begin()->second.avatar.archetype_id()
	    == npc::default_soul_id);
	assert(fallback_backend.players.begin()->second.display_name
	    == "Remote");

	fallback_backend.desired_spawns_succeed = true;
	fallback_backend.spawned_state = remote_avatar_state::pending;
	result = fallback_manager.sync(fallback_players, start + 500ms);
	assert(result.success);
	assert(fallback_backend.players.size() == 1);
	result = fallback_manager.sync(fallback_players, start + 1s);
	assert(result.success);
	assert(fallback_backend.players.size() == 2);
	for (auto &[handle, state] : fallback_backend.states)
	{
		if (fallback_backend.players.at(handle).avatar.archetype_id()
		    == fallback_players[0].avatar.archetype_id())
			state = remote_avatar_state::ready;
	}
	result = fallback_manager.sync(fallback_players, start + 1100ms);
	assert(result.success);
	assert(fallback_backend.players.size() == 1);
	assert(fallback_backend.players.begin()->second.avatar.archetype_id()
	    == fallback_players[0].avatar.archetype_id());

	const auto fallback_handle = fallback_backend.players.begin()->first;
	fallback_backend.states[fallback_handle] =
	    remote_avatar_state::failed;
	result = fallback_manager.sync(fallback_players, start + 2s);
	assert(result.success);
	assert(result.degraded);
	for (auto &[handle, state] : fallback_backend.states)
	{
		if (fallback_backend.players.at(handle).avatar.archetype_id()
		    == npc::default_soul_id)
			state = remote_avatar_state::failed;
	}
	result = fallback_manager.sync(fallback_players, start + 2500ms);
	assert(!result.success);
	assert(result.error.contains("fallback Soul"));
	assert(result.error.contains("player 3"));

	fake_backend backoff_backend;
	backoff_backend.desired_spawns_succeed = false;
	remote_avatar_manager backoff_manager(backoff_backend);
	auto backoff_players = std::vector{
	    player(4, 40.0F, protocol::MOVEMENT_MODE_IDLE)};
	backoff_players[0].avatar.set_archetype_id(
	    "aaaaaaaa-bbbb-4ccc-8ddd-eeeeeeeeeeee");
	const auto backoff_start = remote_avatar_manager::clock::now();
	result = backoff_manager.sync(backoff_players, backoff_start);
	assert(result.success);
	assert(backoff_backend.spawn_attempts == 2);
	for (const auto [elapsed, expected_attempts] :
	     std::array{
	         std::pair{500ms, std::size_t{2}},
	         std::pair{1000ms, std::size_t{3}},
	         std::pair{2000ms, std::size_t{3}},
	         std::pair{3000ms, std::size_t{4}},
	         std::pair{7000ms, std::size_t{5}},
	         std::pair{15000ms, std::size_t{6}},
	         std::pair{31000ms, std::size_t{7}},
	         std::pair{61000ms, std::size_t{8}}})
	{
		result = backoff_manager.sync(
		    backoff_players,
		    backoff_start + elapsed);
		assert(result.success);
		assert(backoff_backend.spawn_attempts == expected_attempts);
	}

	fake_backend default_failure_backend;
	default_failure_backend.fallback_spawns_succeed = false;
	remote_avatar_manager default_failure_manager(
	    default_failure_backend);
	auto default_failure_players = std::vector{
	    player(5, 50.0F, protocol::MOVEMENT_MODE_IDLE)};
	result = default_failure_manager.sync(default_failure_players);
	assert(!result.success);
	assert(result.error.contains("default Soul"));
	assert(default_failure_backend.spawn_attempts == 1);
	assert(default_failure_manager.size() == 0);
	return 0;
}
