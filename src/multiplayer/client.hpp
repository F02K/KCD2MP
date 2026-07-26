#pragma once

#include "multiplayer/game_command_queue.hpp"
#include "multiplayer/identity_store.hpp"
#include "multiplayer/networking.hpp"

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
	};

	class multiplayer_client
	{
	public:
		multiplayer_client();
		~multiplayer_client();
		multiplayer_client(const multiplayer_client &) = delete;
		multiplayer_client &operator=(const multiplayer_client &) = delete;

		[[nodiscard]] bool connect(client_options options);
		void disconnect();
		[[nodiscard]] bool send_chat(std::string text);
		void game_tick(
		    std::optional<protocol::TransformState> local_transform,
		    std::string_view current_level,
		    std::chrono::steady_clock::time_point now =
		        std::chrono::steady_clock::now());

		[[nodiscard]] client_status status() const;
		[[nodiscard]] std::vector<remote_player_view> remote_players() const;
		[[nodiscard]] std::vector<chat_entry> chat_history() const;
		[[nodiscard]] std::optional<protocol::TransformState> take_local_correction();

	private:
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
		using network_command = std::variant<
		    connect_command,
		    disconnect_command,
		    transform_command,
		    chat_command,
		    world_ready_command,
		    world_failed_command,
		    profile_command>;

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
		std::optional<protocol::ServerBootstrap> m_pending_bootstrap;
		bool m_profile_update_pending{};
		std::uint32_t m_profile_snapshot_interval_seconds{15};

		mutable std::mutex m_network_mutex;
		std::deque<network_command> m_network_commands;
		game_command_queue m_game_commands;
		std::jthread m_network_thread;
		std::chrono::steady_clock::time_point m_last_transform_sent{};
		std::chrono::steady_clock::time_point m_last_profile_sent{};
	};

	inline multiplayer_client *g_multiplayer_client{};
	[[nodiscard]] const char *to_string(client_state state);
}
