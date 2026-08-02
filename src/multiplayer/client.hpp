#pragma once

#include "multiplayer/game_command_queue.hpp"
#include "multiplayer/identity_store.hpp"
#include "multiplayer/networking.hpp"
#include "multiplayer/runtime.hpp"

#include <chrono>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <variant>
#include <vector>

namespace kcd2mp
{
	enum class client_state
	{
		disconnected,
		preflight,
		authenticating,
		waiting_for_bootstrap,
		loading_sandbox,
		applying_profile,
		connected,
		reconnecting,
		closing
	};

	struct client_options
	{
		std::string address{"127.0.0.1:27020"};
		std::string display_name{"Henry"};
		std::string password;
		std::string content_hash;
		std::string claim_code;
	};

	struct chat_entry
	{
		player_id sender{};
		std::string display_name;
		std::string text;
		std::uint64_t server_time_ms{};
	};

	struct remote_player_view
	{
		player_id id{};
		std::string display_name;
		bool connected{};
		bool has_transform{};
		protocol::TransformState transform;
		protocol::MovementMode movement_mode{protocol::MOVEMENT_MODE_IDLE};
		protocol::AvatarDescriptor avatar;
		bool has_avatar{};
	};

	struct client_status
	{
		client_state state{client_state::disconnected};
		player_id local_player_id{};
		std::string server_name;
		std::string server_id;
		std::string session_id;
		std::string level_id;
		std::string error;
		int ping_ms{-1};
		float packet_loss_percent{};
		std::size_t game_queue_size{};
		protocol::AvatarPolicy avatar_policy;
		std::string avatar_archetype_id;
	};

	class multiplayer_client
	{
	public:
		explicit multiplayer_client(client_runtime &runtime);
		~multiplayer_client();
		multiplayer_client(const multiplayer_client &) = delete;
		multiplayer_client &operator=(const multiplayer_client &) = delete;

		[[nodiscard]] bool connect(client_options options);
		void disconnect();
		void fail(std::string error);
		[[nodiscard]] bool send_chat(std::string text);
		[[nodiscard]] bool select_avatar(std::string archetype_id);
		void runtime_epoch_changed();
		[[nodiscard]] bool reserve_local_avatar_sample(
		    std::chrono::steady_clock::time_point now =
		        std::chrono::steady_clock::now());
		void game_tick(
		    std::optional<protocol::TransformState> local_transform,
		    std::optional<protocol::AvatarDescriptor> local_avatar_visual,
		    std::string_view current_level,
		    std::chrono::steady_clock::time_point now =
		        std::chrono::steady_clock::now());

		[[nodiscard]] client_status status() const;
		[[nodiscard]] std::vector<remote_player_view> remote_players() const;
		[[nodiscard]] std::vector<chat_entry> chat_history() const;
		[[nodiscard]] std::optional<protocol::TransformState> take_local_correction();

	private:
		client_runtime &m_runtime;

		struct connect_command
		{
			client_options options;
		};
		struct disconnect_command
		{
		};
		struct transform_command
		{
			protocol::TransformState transform;
		};
		struct chat_command
		{
			std::string text;
		};
		struct world_ready_command
		{
			protocol::ClientWorldReady message;
		};
		struct world_failed_command
		{
			protocol::ClientWorldFailed message;
		};
		struct profile_command
		{
			protocol::ClientProfileUpdate message;
		};
		struct avatar_command
		{
			protocol::ClientAvatarUpdate message;
		};
		using network_command = std::variant<
		    connect_command,
		    disconnect_command,
		    transform_command,
		    chat_command,
		    world_ready_command,
		    world_failed_command,
		    profile_command,
		    avatar_command>;

		struct timed_transform
		{
			std::chrono::steady_clock::time_point received_at;
			protocol::TransformState transform;
			protocol::MovementMode mode{protocol::MOVEMENT_MODE_IDLE};
			bool connected{};
		};

		struct remote_player
		{
			std::string display_name;
			std::deque<timed_transform> history;
			remote_player_view rendered;
		};

		void network_loop(std::stop_token stop);
		void advance_runtime_preflight();
		void ensure_network_thread();
		void set_state(client_state state, std::string error = {});
		void queue_network(network_command command);
		void queue_profile_snapshot(protocol::PlayerProfile profile);
		void handle_game_envelope(
		    const protocol::Envelope &envelope,
		    std::chrono::steady_clock::time_point now);
		void advance_sandbox_bootstrap();
		void update_interpolation(std::chrono::steady_clock::time_point now);
		void accept_snapshot_player(
		    const protocol::PlayerSnapshot &snapshot,
		    std::chrono::steady_clock::time_point now);
		[[nodiscard]] static protocol::TransformState interpolate(
		    const protocol::TransformState &from,
		    const protocol::TransformState &to,
		    float factor);
		[[nodiscard]] static protocol::TransformState extrapolate(
		    const protocol::TransformState &from,
		    float seconds);

		mutable std::mutex m_state_mutex;
		client_status m_status;
		std::string m_resume_token;
		std::string m_server_id;
		identity_store m_identities;
		std::unordered_map<player_id, remote_player> m_remote_players;
		std::deque<chat_entry> m_chat;
		std::optional<protocol::TransformState> m_local_correction;
		std::optional<protocol::PlayerProfile> m_profile;
		std::optional<protocol::PlayerProfile> m_pending_profile;
		std::optional<protocol::AvatarDescriptor> m_local_avatar;
		std::optional<protocol::AvatarDescriptor> m_pending_avatar;
		std::optional<protocol::AvatarDescriptor> m_desired_avatar;
		std::optional<std::string> m_desired_archetype;
		bool m_avatar_update_pending{};
		std::optional<protocol::ServerBootstrap> m_pending_bootstrap;
		std::optional<client_options> m_pending_connect;
		bool m_profile_update_pending{};
		std::uint32_t m_profile_snapshot_interval_seconds{15};

		mutable std::mutex m_network_mutex;
		std::deque<network_command> m_network_commands;
		game_command_queue m_game_commands;
		std::jthread m_network_thread;
		std::chrono::steady_clock::time_point m_last_transform_sent{};
		std::chrono::steady_clock::time_point m_last_profile_sent{};
		std::chrono::steady_clock::time_point m_last_avatar_sent{};
		std::chrono::steady_clock::time_point m_last_avatar_sampled{};
	};

	[[nodiscard]] inline const char *to_string(client_state state)
	{
		switch (state)
		{
		case client_state::disconnected:
			return "Disconnected";
		case client_state::preflight:
			return "Preflight";
		case client_state::authenticating:
			return "Authenticating";
		case client_state::waiting_for_bootstrap:
			return "Waiting for bootstrap";
		case client_state::loading_sandbox:
			return "Loading sandbox";
		case client_state::applying_profile:
			return "Applying profile";
		case client_state::connected:
			return "Connected";
		case client_state::reconnecting:
			return "Reconnecting";
		case client_state::closing:
			return "Closing";
		}
		return "Unknown";
	}
}
