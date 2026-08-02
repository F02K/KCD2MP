#pragma once

#include "multiplayer/protocol.hpp"
#include "server/server_config.hpp"
#include "server/world_store.hpp"

#include <chrono>
#include <deque>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace kcd2mp::server
{
	using clock = std::chrono::steady_clock;
	using time_point = clock::time_point;

	enum class close_kind
	{
		none,
		reject,
		shutdown,
		kick
	};

	struct outbound_message
	{
		connection_id connection{};
		protocol::Envelope envelope;
		reliability delivery{reliability::reliable};
		close_kind close_after_send{close_kind::none};
	};

	struct player_view
	{
		player_id id{};
		std::string display_name;
		bool connected{};
		bool has_transform{};
		std::uint64_t last_sequence{};
		protocol::MovementMode movement_mode{protocol::MOVEMENT_MODE_IDLE};
		bool dummy{};
	};

	class server_core
	{
	public:
		using token_generator = std::function<std::string()>;

		explicit server_core(
		    server_config config,
		    token_generator generate_token = {});

		void on_transport_connected(connection_id connection, time_point now);
		void on_transport_disconnected(
		    connection_id connection,
		    bool allow_reconnect,
		    std::string reason,
		    time_point now);
		void on_message(
		    connection_id connection,
		    const protocol::Envelope &envelope,
		    time_point now);
		void tick(time_point now);
		void kick(player_id id, std::string reason, time_point now);
		[[nodiscard]] std::optional<player_id> spawn_dummy(
		    std::string display_name,
		    std::string *error = nullptr);
		[[nodiscard]] bool remove_dummy(player_id id, time_point now);
		void server_say(std::string text, time_point now);
		[[nodiscard]] bool set_npc_entities_disabled(
		    bool humans_disabled,
		    bool animals_disabled);
		[[nodiscard]] bool human_npcs_disabled() const;
		[[nodiscard]] bool animal_npcs_disabled() const;
		void shutdown(std::string reason);
		[[nodiscard]] std::optional<std::string> create_profile_claim(
		    player_id id,
		    time_point now);

		[[nodiscard]] std::vector<outbound_message> take_outbound();
		[[nodiscard]] std::vector<player_view> players() const;
		[[nodiscard]] std::size_t pending_connection_count() const;
		[[nodiscard]] std::uint64_t server_tick() const;
		[[nodiscard]] const server_config &config() const;

	private:
		enum class pending_stage
		{
			hello,
			authenticate,
			waiting_for_initializer,
			loading_world
		};

		struct pending_connection
		{
			time_point connected_at;
			time_point deadline;
			pending_stage stage{pending_stage::hello};
			std::string display_name;
			std::string content_hash;
			std::optional<persisted_profile> persisted;
			std::string issued_identity_token;
			std::string resume_token;
			bool initializer{};
		};

		struct player_session
		{
			player_id id{};
			std::string display_name;
			std::string resume_token;
			token_hash identity_hash{};
			std::optional<connection_id> connection;
			bool dummy{};
			bool has_transform{};
			protocol::TransformState transform;
			protocol::MovementMode movement_mode{protocol::MOVEMENT_MODE_IDLE};
			std::uint64_t last_sequence{};
			time_point last_message_at;
			time_point last_transform_at;
			time_point reconnect_deadline;
			std::deque<time_point> chat_times;
			std::deque<time_point> avatar_update_times;
			protocol::AvatarDescriptor avatar;
			protocol::PlayerProfile profile;
			time_point last_persisted_at;
		};

		struct profile_claim
		{
			token_hash code_hash{};
			time_point expires_at;
		};

		void handle_hello(
		    connection_id connection,
		    const protocol::ClientHello &hello,
		    time_point now);
		void handle_authenticate(
		    connection_id connection,
		    const protocol::ClientAuthenticate &message,
		    time_point now);
		void handle_world_ready(
		    connection_id connection,
		    const protocol::ClientWorldReady &message,
		    time_point now);
		void handle_world_failed(
		    connection_id connection,
		    const protocol::ClientWorldFailed &message,
		    time_point now);
		void handle_profile_update(
		    player_session &player,
		    const protocol::ClientProfileUpdate &message,
		    time_point now);
		void handle_transform(
		    player_session &player,
		    const protocol::ClientTransform &message,
		    time_point now);
		void handle_avatar_update(
		    player_session &player,
		    const protocol::ClientAvatarUpdate &message,
		    time_point now);
		void handle_chat(
		    player_session &player,
		    const protocol::ChatSend &message,
		    time_point now);
		void handle_ping(
		    player_session &player,
		    const protocol::Ping &message,
		    time_point now);
		void reject(
		    connection_id connection,
		    protocol::RejectReason reason,
		    std::string message);
		void remove_player(
		    player_id id,
		    std::string reason,
		    close_kind close,
		    time_point now);
		void send_accepted(player_session &player);
		void send_entity_control(connection_id connection);
		void apply_default_avatar(protocol::PlayerProfile &profile);
		[[nodiscard]] bool avatar_allowed(
		    const protocol::AvatarDescriptor &avatar) const;
		[[nodiscard]] protocol::AvatarPolicy avatar_policy() const;
		void send_challenge(
		    connection_id connection,
		    std::uint64_t client_features);
		void send_bootstrap(connection_id connection, protocol::BootstrapMode mode);
		void release_initializer(connection_id connection);
		void wake_bootstrap_waiters();
		void persist_player(player_session &player, time_point now);
		void broadcast(
		    protocol::Envelope envelope,
		    reliability delivery,
		    std::optional<connection_id> except = std::nullopt);
		void queue(
		    connection_id connection,
		    protocol::Envelope envelope,
		    reliability delivery,
		    close_kind close = close_kind::none);
		void queue_snapshot(time_point now);
		[[nodiscard]] player_session *find_by_connection(connection_id connection);
		[[nodiscard]] player_session *find_by_resume_token(std::string_view token);
		[[nodiscard]] static std::string lower_ascii(std::string_view value);
		[[nodiscard]] static std::uint64_t milliseconds(time_point value);
		[[nodiscard]] static protocol::PlayerSnapshot snapshot_of(
		    const player_session &player,
		    bool include_avatar);

		server_config m_config;
		token_generator m_generate_token;
		world_store m_store;
		std::uint64_t m_server_tick{};
		time_point m_last_snapshot{};
		std::unordered_map<connection_id, pending_connection> m_pending;
		std::unordered_map<player_id, player_session> m_players;
		std::unordered_map<player_id, profile_claim> m_claims;
		std::optional<connection_id> m_initializer;
		std::uint64_t m_next_dummy_index{1};
		bool m_human_npcs_disabled{};
		bool m_animal_npcs_disabled{};
		std::vector<outbound_message> m_outbound;
	};
}
