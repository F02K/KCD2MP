#include "multiplayer/remote_avatar.hpp"

#include <cassert>
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
			const auto handle = next_handle++;
			players[handle] = player;
			return handle;
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
			++removed;
		}

		bool enabled{true};
		bool updates_succeed{true};
		kcd2mp::remote_avatar_handle next_handle{1};
		std::size_t removed{};
		std::size_t appearance_updates{};
		std::unordered_map<
		    kcd2mp::remote_avatar_handle,
		    kcd2mp::remote_avatar_snapshot>
		    players;
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
	fake_backend backend;
	remote_avatar_manager manager(backend);
	std::vector players{
	    player(1, 10.0F, protocol::MOVEMENT_MODE_IDLE),
	    player(2, 20.0F, protocol::MOVEMENT_MODE_WALK)};

	auto result = manager.sync(players);
	assert(result.success);
	assert(result.spawned == 2);
	assert(manager.size() == 2);

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
	return 0;
}
