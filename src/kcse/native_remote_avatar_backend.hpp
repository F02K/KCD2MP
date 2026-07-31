#pragma once

#include "kcse/native_entity_backend.hpp"
#include "kcse/remote_avatar_readiness.hpp"
#include "multiplayer/remote_avatar.hpp"

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace kcd2mp::kcse
{
	class native_remote_avatar_backend final : public remote_avatar_backend
	{
	public:
		enum class active_probe_result
		{
			pending,
			succeeded,
			failed
		};

		explicit native_remote_avatar_backend(native_entity_backend &entities);

		void advance_frame() noexcept;
		void set_epoch(std::uint64_t epoch);
		void clear();
		void reset_active_probe();
		[[nodiscard]] active_probe_result poll_active_probe(
		    const protocol::TransformState &origin,
		    std::string &error);

		[[nodiscard]] bool available() const override;
		[[nodiscard]] std::string diagnostic() const override;
		[[nodiscard]] std::optional<remote_avatar_handle> spawn(
		    const remote_avatar_snapshot &player) override;
		[[nodiscard]] remote_avatar_backend_status status(
		    remote_avatar_handle avatar) const override;
		[[nodiscard]] bool update(
		    remote_avatar_handle avatar,
		    const remote_avatar_snapshot &player,
		    bool appearance_changed) override;
		void remove(remote_avatar_handle avatar) override;

	private:
		struct entry
		{
			player_id player{};
			std::uint32_t entity_id{};
			std::uint64_t epoch{};
			std::string shared_soul_guid;
			bool shared_soul_applied{};
			std::uint64_t shared_soul_applied_frame{};
			std::chrono::steady_clock::time_point
			    shared_soul_applied_at{};
			bool appearance_applied{};
			bool first_transform_logged{};
			bool first_motion_logged{};
			bool first_weapon_action_logged{};
			bool failed{};
			std::string failure;
			protocol::AvatarDescriptor appearance;
			std::vector<std::string> item_instances;
		};

		[[nodiscard]] entry *find(remote_avatar_handle avatar);
		[[nodiscard]] const entry *find(remote_avatar_handle avatar) const;
		[[nodiscard]] bool apply_appearance(
		    entry &avatar,
		    const protocol::AvatarDescriptor &appearance,
		    std::string &error);
		[[nodiscard]] bool remove_created_items(
		    entry &avatar,
		    std::string &error);
		[[nodiscard]] bool drive_motion(
		    entry &avatar,
		    const remote_avatar_snapshot &player,
		    std::string &error);

		native_entity_backend &m_entities;
		mutable std::unordered_map<remote_avatar_handle, entry> m_avatars;
		std::optional<remote_avatar_handle> m_probe_avatar;
		remote_avatar_snapshot m_probe_snapshot;
		std::uint32_t m_probe_polls{};
		std::uint64_t m_frame_sequence{};
		std::uint64_t m_epoch{1};
		remote_avatar_handle m_next_handle{1};
		mutable std::string m_diagnostic;
	};
}
