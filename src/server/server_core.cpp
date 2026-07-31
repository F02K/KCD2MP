#include "server/server_core.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <stdexcept>

namespace kcd2mp::server
{
	namespace
	{
		std::string lowercase_ascii(std::string_view value)
		{
			std::string result(value);
			std::ranges::transform(
			    result,
			    result.begin(),
			    [](unsigned char character)
			    {
				    return static_cast<char>(std::tolower(character));
			    });
			return result;
		}

		protocol::Envelope player_left_envelope(
		    player_id id,
		    const std::string &reason)
		{
			protocol::Envelope envelope;
			auto *left = envelope.mutable_player_left();
			left->set_player_id(id);
			left->set_reason(reason);
			return envelope;
		}

		bool profile_name_matches(
		    const protocol::PlayerProfile &profile,
		    std::string_view display_name)
		{
			return lowercase_ascii(profile.display_name())
			    == lowercase_ascii(display_name);
		}

		server_config validated_config(server_config config)
		{
			normalize_avatar_config(config);
			validate_server_config(config);
			return config;
		}
	}

	server_core::server_core(
	    server_config config,
	    token_generator generate_token) :
	    m_config(validated_config(std::move(config))),
	    m_generate_token(generate_token ? std::move(generate_token) : []
	        { return random_hex(32); }),
	    m_store(m_config),
	    m_non_player_entities_disabled(
	        m_config.disable_non_player_entities)
	{
	}

	void server_core::on_transport_connected(
	    connection_id connection,
	    time_point now)
	{
		if (connection == 0 || m_pending.contains(connection)
		    || find_by_connection(connection))
		{
			return;
		}
		m_pending.emplace(
		    connection,
		    pending_connection{
		        .connected_at = now,
		        .deadline =
		            now + std::chrono::seconds(
		                m_config.handshake_timeout_seconds)});
	}

	void server_core::on_transport_disconnected(
	    connection_id connection,
	    bool allow_reconnect,
	    std::string reason,
	    time_point now)
	{
		if (m_pending.contains(connection))
		{
			release_initializer(connection);
			m_pending.erase(connection);
			wake_bootstrap_waiters();
		}
		auto *player = find_by_connection(connection);
		if (!player)
		{
			return;
		}
		persist_player(*player, now);
		if (allow_reconnect)
		{
			player->connection.reset();
			player->reconnect_deadline =
			    now + std::chrono::seconds(m_config.reconnect_grace_seconds);
			return;
		}
		remove_player(player->id, std::move(reason), close_kind::none, now);
	}

	void server_core::on_message(
	    connection_id connection,
	    const protocol::Envelope &envelope,
	    time_point now)
	{
		if (auto *player = find_by_connection(connection))
		{
			player->last_message_at = now;
			switch (envelope.payload_case())
			{
			case protocol::Envelope::kClientTransform:
				handle_transform(*player, envelope.client_transform(), now);
				break;
			case protocol::Envelope::kChatSend:
				handle_chat(*player, envelope.chat_send(), now);
				break;
			case protocol::Envelope::kClientProfileUpdate:
				handle_profile_update(
				    *player,
				    envelope.client_profile_update(),
				    now);
				break;
			case protocol::Envelope::kClientAvatarUpdate:
				handle_avatar_update(
				    *player,
				    envelope.client_avatar_update(),
				    now);
				break;
			case protocol::Envelope::kPing:
				handle_ping(*player, envelope.ping(), now);
				break;
			default:
				reject(
				    connection,
				    protocol::REJECT_REASON_MALFORMED_MESSAGE,
				    "message is not valid in the connected state");
				break;
			}
			return;
		}

		auto iterator = m_pending.find(connection);
		if (iterator == m_pending.end())
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_MALFORMED_MESSAGE,
			    "ClientHello must be the first message");
			return;
		}
		auto &pending = iterator->second;
		if (envelope.has_ping() && pending.stage != pending_stage::hello)
		{
			protocol::Envelope pong;
			pong.mutable_pong()->set_nonce(envelope.ping().nonce());
			pong.mutable_pong()->set_client_time_ms(
			    envelope.ping().client_time_ms());
			pong.mutable_pong()->set_server_time_ms(milliseconds(now));
			queue(connection, std::move(pong), reliability::reliable);
			return;
		}
		switch (pending.stage)
		{
		case pending_stage::hello:
			if (!envelope.has_client_hello())
			{
				reject(
				    connection,
				    protocol::REJECT_REASON_MALFORMED_MESSAGE,
				    "ClientHello must be the first message");
			}
			else
			{
				handle_hello(connection, envelope.client_hello(), now);
			}
			break;
		case pending_stage::authenticate:
			if (!envelope.has_client_authenticate())
			{
				reject(
				    connection,
				    protocol::REJECT_REASON_MALFORMED_MESSAGE,
				    "ClientAuthenticate must follow ServerChallenge");
			}
			else
			{
				handle_authenticate(
				    connection,
				    envelope.client_authenticate(),
				    now);
			}
			break;
		case pending_stage::waiting_for_initializer:
		case pending_stage::loading_world:
			if (envelope.has_client_world_ready())
			{
				handle_world_ready(
				    connection,
				    envelope.client_world_ready(),
				    now);
			}
			else if (envelope.has_client_world_failed())
			{
				handle_world_failed(
				    connection,
				    envelope.client_world_failed(),
				    now);
			}
			else
			{
				reject(
				    connection,
				    protocol::REJECT_REASON_MALFORMED_MESSAGE,
				    "client is not ready for gameplay messages");
			}
			break;
		}
	}

	void server_core::tick(time_point now)
	{
		++m_server_tick;
		std::vector<connection_id> expired_pending;
		for (const auto &[connection, pending] : m_pending)
		{
			if (now >= pending.deadline)
			{
				expired_pending.push_back(connection);
			}
		}
		for (const auto connection : expired_pending)
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_BOOTSTRAP_FAILED,
			    "handshake or sandbox bootstrap timed out");
		}

		std::vector<player_id> expired_players;
		for (auto &[id, player] : m_players)
		{
			if (player.dummy)
			{
				continue;
			}
			if (player.has_transform
			    && (player.last_persisted_at == time_point{}
			        || now - player.last_persisted_at >= std::chrono::seconds(5)))
			{
				persist_player(player, now);
			}
			if (!player.connection && now >= player.reconnect_deadline)
			{
				expired_players.push_back(id);
			}
			else if (player.connection
			    && now - player.last_message_at
			        >= std::chrono::seconds(m_config.idle_timeout_seconds))
			{
				expired_players.push_back(id);
			}
		}
		for (const auto id : expired_players)
		{
			remove_player(id, "timed out", close_kind::kick, now);
		}

		for (auto iterator = m_claims.begin(); iterator != m_claims.end();)
		{
			iterator = now >= iterator->second.expires_at
			    ? m_claims.erase(iterator)
			    : std::next(iterator);
		}

		const auto snapshot_interval =
		    std::chrono::duration<double>(1.0 / m_config.snapshot_rate);
		if (m_last_snapshot == time_point{}
		    || now - m_last_snapshot >= snapshot_interval)
		{
			queue_snapshot(now);
			m_last_snapshot = now;
		}
	}

	void server_core::kick(player_id id, std::string reason, time_point now)
	{
		remove_player(id, std::move(reason), close_kind::kick, now);
	}

	std::optional<player_id> server_core::spawn_dummy(
	    std::string display_name,
	    std::string *error)
	{
		if (error)
		{
			error->clear();
		}
		const auto set_error = [&](std::string message)
		{
			if (error)
			{
				*error = std::move(message);
			}
		};
		const auto reserved_slots =
		    m_players.size()
		    + static_cast<std::size_t>(std::ranges::count_if(
		        m_pending,
		        [](const auto &entry)
		        {
			        return entry.second.persisted.has_value();
		        }));
		if (reserved_slots >= m_config.max_players)
		{
			set_error("server is full");
			return std::nullopt;
		}

		const auto name_in_use = [&](std::string_view candidate)
		{
			return std::ranges::any_of(
			           m_players,
			           [&](const auto &entry)
			           {
				           return lower_ascii(entry.second.display_name)
				               == lower_ascii(candidate);
			           })
			    || std::ranges::any_of(
			        m_pending,
			        [&](const auto &entry)
			        {
				        return lower_ascii(entry.second.display_name)
				            == lower_ascii(candidate);
			        });
		};
		if (display_name.empty())
		{
			do
			{
				display_name =
				    "Dummy " + std::to_string(m_next_dummy_index++);
			} while (name_in_use(display_name));
		}
		if (!is_valid_display_name(display_name))
		{
			set_error(
			    "display name must contain 3 to 32 UTF-8 characters");
			return std::nullopt;
		}
		if (name_in_use(display_name))
		{
			set_error("display name is already in use");
			return std::nullopt;
		}

		const player_session *anchor = nullptr;
		for (const auto &[id, player] : m_players)
		{
			if (player.dummy || !player.connection
			    || !player.has_transform)
			{
				continue;
			}
			if (!anchor || id < anchor->id)
			{
				anchor = &player;
			}
		}
		protocol::TransformState transform;
		if (anchor)
		{
			transform = anchor->transform;
		}
		else if (m_store.manifest().spawn_valid)
		{
			transform = m_store.manifest().spawn;
		}
		else
		{
			set_error(
			    "no player transform or configured world spawn is available");
			return std::nullopt;
		}
		const auto dummy_count = std::ranges::count_if(
		    m_players,
		    [](const auto &entry)
		    {
			    return entry.second.dummy;
		    });
		transform.mutable_position()->set_x(
		    transform.position().x()
		    + 2.0F * static_cast<float>(dummy_count + 1));
		transform.mutable_velocity()->Clear();
		transform.set_sequence(1);
		transform.set_client_time_ms(0);

		player_session dummy;
		dummy.id = m_store.allocate_player_id();
		dummy.display_name = std::move(display_name);
		dummy.dummy = true;
		dummy.has_transform = true;
		dummy.transform = std::move(transform);
		dummy.last_sequence = 1;
		dummy.movement_mode = protocol::MOVEMENT_MODE_IDLE;
		dummy.avatar.set_archetype_id(
		    m_config.default_avatar_archetype);
		dummy.avatar.set_revision(1);
		dummy.avatar.set_stance(protocol::AVATAR_STANCE_RELAXED);
		dummy.avatar.set_weapon_class(
		    protocol::AVATAR_WEAPON_CLASS_NONE);

		const auto id = dummy.id;
		auto [iterator, inserted] =
		    m_players.emplace(id, std::move(dummy));
		(void)inserted;
		protocol::Envelope joined;
		*joined.mutable_player_joined()->mutable_player() =
		    snapshot_of(iterator->second, true);
		broadcast(std::move(joined), reliability::reliable);
		return id;
	}

	bool server_core::remove_dummy(player_id id, time_point now)
	{
		const auto iterator = m_players.find(id);
		if (iterator == m_players.end() || !iterator->second.dummy)
		{
			return false;
		}
		remove_player(id, "dummy removed", close_kind::none, now);
		return true;
	}

	void server_core::server_say(std::string text, time_point now)
	{
		if (!is_valid_chat(text))
		{
			return;
		}
		protocol::Envelope envelope;
		auto *chat = envelope.mutable_chat_broadcast();
		chat->set_player_id(0);
		chat->set_display_name("Server");
		chat->set_text(std::move(text));
		chat->set_server_time_ms(milliseconds(now));
		broadcast(std::move(envelope), reliability::reliable);
	}

	bool server_core::set_non_player_entities_disabled(bool disabled)
	{
		if (m_non_player_entities_disabled == disabled)
		{
			return false;
		}
		m_non_player_entities_disabled = disabled;
		protocol::Envelope envelope;
		envelope.mutable_server_entity_control()
		    ->set_non_player_entities_disabled(disabled);
		broadcast(std::move(envelope), reliability::reliable);
		return true;
	}

	bool server_core::non_player_entities_disabled() const
	{
		return m_non_player_entities_disabled;
	}

	void server_core::shutdown(std::string reason)
	{
		const auto now = clock::now();
		for (auto &[id, player] : m_players)
		{
			(void)id;
			if (!player.dummy)
			{
				persist_player(player, now);
			}
			if (!player.connection)
			{
				continue;
			}
			protocol::Envelope envelope;
			envelope.mutable_server_shutdown()->set_reason(reason);
			queue(
			    *player.connection,
			    std::move(envelope),
			    reliability::reliable,
			    close_kind::shutdown);
		}
		m_players.clear();
		m_pending.clear();
		m_initializer.reset();
	}

	std::optional<std::string> server_core::create_profile_claim(
	    player_id id,
	    time_point now)
	{
		if (!m_store.find_by_player_id(id))
		{
			return std::nullopt;
		}
		const auto code = random_hex(16);
		m_claims[id] = {hash_token(code), now + std::chrono::minutes(10)};
		return code;
	}

	std::vector<outbound_message> server_core::take_outbound()
	{
		auto result = std::move(m_outbound);
		m_outbound.clear();
		return result;
	}

	std::vector<player_view> server_core::players() const
	{
		std::vector<player_view> result;
		result.reserve(m_players.size());
		for (const auto &[id, player] : m_players)
		{
			result.push_back({
			    id,
			    player.display_name,
			    player.dummy || player.connection.has_value(),
			    player.has_transform,
			    player.last_sequence,
			    player.movement_mode,
			    player.dummy});
		}
		std::ranges::sort(result, {}, &player_view::id);
		return result;
	}

	std::size_t server_core::pending_connection_count() const
	{
		return m_pending.size();
	}

	std::uint64_t server_core::server_tick() const
	{
		return m_server_tick;
	}

	const server_config &server_core::config() const
	{
		return m_config;
	}

	void server_core::handle_hello(
	    connection_id connection,
	    const protocol::ClientHello &hello,
	    time_point now)
	{
		auto reject_hello = [&](protocol::RejectReason reason, std::string message)
		{
			reject(connection, reason, std::move(message));
		};
		if (hello.protocol_version() != protocol_version
		    || hello.client_version() != version_string)
		{
			reject_hello(
			    protocol::REJECT_REASON_PROTOCOL_MISMATCH,
			    "KCD2MP protocol or version mismatch");
			return;
		}
		if (hello.whgame_timestamp() != supported_whgame_timestamp
		    || hello.whgame_image_size() != supported_whgame_image_size)
		{
			reject_hello(
			    protocol::REJECT_REASON_GAME_BUILD_MISMATCH,
			    "unsupported WHGame build");
			return;
		}
		if (!hello.has_runtime()
		    || (hello.runtime().features()
		            & required_client_runtime_capabilities)
		        != required_client_runtime_capabilities
		    || hello.runtime().kcse_version() == 0
		    || hello.runtime().game_version()
		        != supported_kcse_game_version
		    || hello.runtime().release_index()
		        != supported_kcse_release_index
		    || hello.runtime().address_library().empty())
		{
			reject_hello(
			    protocol::REJECT_REASON_GAME_BUILD_MISMATCH,
			    "KCSE/libKCD2 runtime capabilities are incomplete");
			return;
		}
		if (hello.password() != m_config.password)
		{
			reject_hello(
			    protocol::REJECT_REASON_AUTHENTICATION_FAILED,
			    "authentication failed");
			return;
		}
		if (!m_config.required_content_hash.empty()
		    && hello.content_hash() != m_config.required_content_hash)
		{
			reject_hello(
			    protocol::REJECT_REASON_CONTENT_MISMATCH,
			    "content hash mismatch");
			return;
		}
		if (!is_valid_display_name(hello.display_name()))
		{
			reject_hello(
			    protocol::REJECT_REASON_MALFORMED_MESSAGE,
			    "display name must contain 3 to 32 UTF-8 characters");
			return;
		}
		auto &pending = m_pending.at(connection);
		pending.display_name = hello.display_name();
		pending.content_hash = hello.content_hash();
		pending.stage = pending_stage::authenticate;
		pending.deadline =
		    now + std::chrono::seconds(m_config.handshake_timeout_seconds);
		send_challenge(connection);
	}

	void server_core::handle_authenticate(
	    connection_id connection,
	    const protocol::ClientAuthenticate &message,
	    time_point now)
	{
		auto &pending = m_pending.at(connection);
		const auto reserved_slots = [&]
		{
			return m_players.size()
			    + static_cast<std::size_t>(std::ranges::count_if(
			        m_pending,
			        [&](const auto &entry)
			        {
				        return entry.first != connection
				            && entry.second.persisted.has_value();
			        }));
		};
		if (message.enroll()
		    && reserved_slots() >= m_config.max_players)
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_SERVER_FULL,
			    "server is full");
			return;
		}
		std::optional<persisted_profile> profile;
		bool rotate_identity = false;
		bool enrolled_profile = false;
		if (!message.identity_token().empty())
		{
			profile = m_store.find_by_token(message.identity_token());
			if (!profile)
			{
				reject(
				    connection,
				    protocol::REJECT_REASON_IDENTITY_REQUIRED,
				    "identity token is unknown");
				return;
			}
		}
		else if (!message.claim_code().empty())
		{
			const auto candidate = hash_token(message.claim_code());
			for (auto iterator = m_claims.begin(); iterator != m_claims.end(); ++iterator)
			{
				if (now < iterator->second.expires_at
				    && secure_equal(candidate, iterator->second.code_hash))
				{
					profile = m_store.find_by_player_id(iterator->first);
					m_claims.erase(iterator);
					break;
				}
			}
			if (!profile)
			{
				reject(
				    connection,
				    protocol::REJECT_REASON_IDENTITY_REQUIRED,
				    "claim code is invalid or expired");
				return;
			}
			rotate_identity = true;
		}
		else if (message.enroll())
		{
			const auto duplicate = std::ranges::any_of(
			    m_store.profiles(),
			    [&](const persisted_profile &stored)
			    {
				    return profile_name_matches(
				        stored.profile,
				        pending.display_name);
			    });
			if (duplicate)
			{
				reject(
				    connection,
				    protocol::REJECT_REASON_IDENTITY_REQUIRED,
				    "an identity token is required for this player profile");
				return;
			}
			protocol::PlayerProfile created = instantiate_starter_profile(
			    m_config.starter_profile,
			    m_store.allocate_player_id(),
			    pending.display_name,
			    m_store.manifest().level_id);
			apply_default_avatar(created);
			if (m_store.manifest().spawn_valid)
			{
				created.set_transform_valid(true);
				*created.mutable_last_transform() = m_store.manifest().spawn;
			}
			pending.issued_identity_token = m_generate_token();
			profile = persisted_profile{
			    hash_token(pending.issued_identity_token),
			    std::move(created)};
			enrolled_profile = true;
			m_store.save_profile(profile->identity_hash, profile->profile);
		}
		if (!profile)
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_IDENTITY_REQUIRED,
			    "identity credentials are required");
			return;
		}
		// Enrollment is persisted before the initializer has applied its native
		// profile. If that first bootstrap fails, rebuild the untouched revision-1
		// profile from the current starter template on retry. This also migrates
		// profiles created with an older, non-native starter-item definition.
		if (!enrolled_profile && !m_store.manifest().spawn_valid
		    && profile->profile.revision() == 1
		    && !profile->profile.transform_valid())
		{
			auto refreshed = instantiate_starter_profile(
			    m_config.starter_profile,
			    profile->profile.player_id(),
			    profile->profile.display_name(),
			    m_store.manifest().level_id);
			apply_default_avatar(refreshed);
			profile->profile = std::move(refreshed);
			m_store.save_profile(profile->identity_hash, profile->profile);
		}
		if (!profile->profile.has_avatar()
		    || !is_valid_avatar_descriptor(profile->profile.avatar()))
		{
			apply_default_avatar(profile->profile);
			m_store.save_profile(profile->identity_hash, profile->profile);
		}
		else if (!avatar_allowed(profile->profile.avatar()))
		{
			profile->profile.mutable_avatar()->set_archetype_id(
			    m_config.default_avatar_archetype);
			m_store.save_profile(profile->identity_hash, profile->profile);
		}
		if (!profile_name_matches(profile->profile, pending.display_name))
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_IDENTITY_REQUIRED,
			    "display name does not match the persistent profile");
			return;
		}
		const auto id = profile->profile.player_id();
		if (!m_players.contains(id)
		    && reserved_slots() >= m_config.max_players)
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_SERVER_FULL,
			    "server is full");
			return;
		}
		const auto active = m_players.contains(id)
		    && m_players.at(id).connection.has_value();
		const auto authenticating = std::ranges::any_of(
		    m_pending,
		    [&](const auto &entry)
		    {
			    return entry.first != connection && entry.second.persisted
			        && entry.second.persisted->profile.player_id() == id;
		    });
		if (active || authenticating)
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_IDENTITY_IN_USE,
			    "player identity is already connected");
			return;
		}
		if (rotate_identity)
		{
			pending.issued_identity_token = m_generate_token();
			profile->identity_hash = hash_token(pending.issued_identity_token);
			m_store.save_profile(profile->identity_hash, profile->profile);
		}
		pending.persisted = std::move(profile);
		pending.resume_token = message.resume_token();
		pending.deadline =
		    now + std::chrono::seconds(m_config.bootstrap_timeout_seconds);
		if (m_store.manifest().spawn_valid)
		{
			pending.stage = pending_stage::loading_world;
			send_bootstrap(connection, protocol::BOOTSTRAP_MODE_LOAD);
		}
		else if (!m_initializer)
		{
			m_initializer = connection;
			pending.initializer = true;
			pending.stage = pending_stage::loading_world;
			send_bootstrap(connection, protocol::BOOTSTRAP_MODE_INITIALIZE);
		}
		else
		{
			pending.stage = pending_stage::waiting_for_initializer;
			send_bootstrap(connection, protocol::BOOTSTRAP_MODE_WAIT);
		}
	}

	void server_core::handle_world_ready(
	    connection_id connection,
	    const protocol::ClientWorldReady &message,
	    time_point now)
	{
		auto &pending = m_pending.at(connection);
		const auto manifest_revision = m_store.manifest().revision;
		if (!pending.persisted
		    || message.session_id() != m_store.manifest().session_id
		    || message.manifest_revision() != manifest_revision
		    || message.level_id() != m_store.manifest().level_id)
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_SESSION_MISMATCH,
			    "client loaded a stale or different sandbox session");
			return;
		}
		auto candidate_profile = pending.persisted->profile;
		if (message.has_avatar())
			*candidate_profile.mutable_avatar() = message.avatar();
		if (!message.has_avatar()
		    || !is_valid_avatar_descriptor(message.avatar())
		    || !is_valid_profile(candidate_profile)
		    || !avatar_allowed(message.avatar()))
		{
			reject(
			    connection,
			    protocol::REJECT_REASON_CONTENT_MISMATCH,
			    "client supplied an invalid or disallowed avatar");
			return;
		}
		if (pending.initializer)
		{
			if (!message.initialized_session() || !message.has_initial_spawn())
			{
				reject(
				    connection,
				    protocol::REJECT_REASON_BOOTSTRAP_FAILED,
				    "initializer did not provide a valid spawn");
				return;
			}
			auto spawn = message.initial_spawn();
			if (!is_finite_transform(spawn)
			    || !normalize_rotation(spawn.mutable_rotation()))
			{
				reject(
				    connection,
				    protocol::REJECT_REASON_BOOTSTRAP_FAILED,
				    "initializer spawn is invalid");
				return;
			}
			m_store.set_spawn(spawn);
			pending.persisted->profile.set_transform_valid(true);
			*pending.persisted->profile.mutable_last_transform() = spawn;
			m_store.save_profile(
			    pending.persisted->identity_hash,
			    pending.persisted->profile);
			m_initializer.reset();
		}

		auto persisted = *pending.persisted;
		m_pending.erase(connection);

		player_session session;
		session.id = persisted.profile.player_id();
		session.display_name = persisted.profile.display_name();
		session.resume_token = m_generate_token();
		session.identity_hash = persisted.identity_hash;
		session.connection = connection;
		session.profile = std::move(persisted.profile);
		session.avatar = message.avatar();
		if (!avatar_allowed(session.avatar))
			session.avatar.set_archetype_id(
			    m_config.default_avatar_archetype);
		session.avatar.set_revision(
		    std::max<std::uint64_t>(
		        1,
		        session.profile.avatar().revision()));
		*session.profile.mutable_avatar() = session.avatar;
		session.last_message_at = now;
		session.last_transform_at = now;
		session.last_persisted_at = now;
		if (session.profile.transform_valid())
		{
			session.has_transform = true;
			session.transform = session.profile.last_transform();
			// Sequences are scoped to a transport/client process. Persist the
			// pose, but start sequence validation fresh after authentication.
			session.last_sequence = 0;
		}
		auto [iterator, inserted] =
		    m_players.insert_or_assign(session.id, std::move(session));
		(void)inserted;
		send_accepted(iterator->second);
		protocol::Envelope joined;
		*joined.mutable_player_joined()->mutable_player() =
		    snapshot_of(iterator->second, true);
		broadcast(std::move(joined), reliability::reliable, connection);
		wake_bootstrap_waiters();
	}

	void server_core::handle_world_failed(
	    connection_id connection,
	    const protocol::ClientWorldFailed &message,
	    time_point now)
	{
		(void)now;
		reject(
		    connection,
		    protocol::REJECT_REASON_BOOTSTRAP_FAILED,
		    "client sandbox bootstrap failed: " + message.reason());
	}

	void server_core::handle_profile_update(
	    player_session &player,
	    const protocol::ClientProfileUpdate &message,
	    time_point now)
	{
		if (!message.has_profile() || !is_valid_profile(message.profile())
		    || message.base_revision() != player.profile.revision()
		    || message.profile().player_id() != player.id)
		{
			protocol::Envelope rejected;
			auto *response = rejected.mutable_profile_rejected();
			response->set_authoritative_revision(player.profile.revision());
			response->set_reason("profile revision or schema conflict");
			queue(
			    *player.connection,
			    std::move(rejected),
			    reliability::reliable,
			    close_kind::reject);
			return;
		}
		auto accepted = message.profile();
		accepted.set_player_id(player.id);
		accepted.set_display_name(player.display_name);
		accepted.set_level_id(m_store.manifest().level_id);
		accepted.set_revision(player.profile.revision() + 1);
		if (accepted.has_avatar()
		    && !avatar_allowed(accepted.avatar()))
		{
			accepted.mutable_avatar()->set_archetype_id(
			    m_config.default_avatar_archetype);
		}
		accepted.set_transform_valid(player.has_transform);
		if (player.has_transform)
		{
			*accepted.mutable_last_transform() = player.transform;
		}
		player.profile = std::move(accepted);
		persist_player(player, now);
		protocol::Envelope response;
		response.mutable_profile_accepted()->set_revision(
		    player.profile.revision());
		queue(*player.connection, std::move(response), reliability::reliable);
	}

	void server_core::handle_transform(
	    player_session &player,
	    const protocol::ClientTransform &message,
	    time_point now)
	{
		if (!message.has_transform())
		{
			reject(
			    *player.connection,
			    protocol::REJECT_REASON_MALFORMED_MESSAGE,
			    "ClientTransform has no transform");
			return;
		}
		auto candidate = message.transform();
		if (!is_finite_transform(candidate)
		    || !normalize_rotation(candidate.mutable_rotation()))
		{
			reject(
			    *player.connection,
			    protocol::REJECT_REASON_MALFORMED_MESSAGE,
			    "transform contains invalid values");
			return;
		}
		if (candidate.sequence() <= player.last_sequence)
		{
			return;
		}
		if (player.has_transform)
		{
			const auto seconds = std::clamp(
			    std::chrono::duration<float>(now - player.last_transform_at).count(),
			    0.001F,
			    1.0F);
			const auto &from = player.transform.position();
			const auto &to = candidate.position();
			const auto distance = std::sqrt(
			    std::pow(to.x() - from.x(), 2.0F)
			    + std::pow(to.y() - from.y(), 2.0F)
			    + std::pow(to.z() - from.z(), 2.0F));
			const auto allowed =
			    m_config.max_player_speed_mps * seconds
			    + m_config.movement_tolerance_m;
			if (distance > allowed)
			{
				protocol::Envelope correction;
				*correction.mutable_state_correction()
				     ->mutable_accepted_transform() = player.transform;
				correction.mutable_state_correction()->set_reason(
				    "movement exceeded the server limit");
				queue(
				    *player.connection,
				    std::move(correction),
				    reliability::reliable);
				return;
			}
		}
		player.transform = std::move(candidate);
		player.last_sequence = player.transform.sequence();
		player.last_transform_at = now;
		player.has_transform = true;
		player.movement_mode = movement_mode_for(player.transform);
	}

	void server_core::handle_avatar_update(
	    player_session &player,
	    const protocol::ClientAvatarUpdate &message,
	    time_point now)
	{
		const auto cutoff = now - std::chrono::seconds(1);
		while (!player.avatar_update_times.empty()
		    && player.avatar_update_times.front() < cutoff)
		{
			player.avatar_update_times.pop_front();
		}
		if (player.avatar_update_times.size() >= 4)
		{
			return;
		}
		player.avatar_update_times.push_back(now);

		auto candidate_profile = player.profile;
		if (message.has_avatar())
			*candidate_profile.mutable_avatar() = message.avatar();
		if (!message.has_avatar()
		    || !is_valid_avatar_descriptor(message.avatar())
		    || !is_valid_profile(candidate_profile))
		{
			protocol::Envelope rejected;
			auto *response = rejected.mutable_avatar_rejected();
			*response->mutable_authoritative_avatar() = player.avatar;
			response->set_reason(
			    "avatar descriptor is invalid or disallowed");
			queue(
			    *player.connection,
			    std::move(rejected),
			    reliability::reliable);
			return;
		}
		if (message.base_revision() != player.avatar.revision())
		{
			protocol::Envelope rejected;
			auto *response = rejected.mutable_avatar_rejected();
			*response->mutable_authoritative_avatar() = player.avatar;
			response->set_reason("avatar revision conflict");
			queue(
			    *player.connection,
			    std::move(rejected),
			    reliability::reliable);
			return;
		}

		player.avatar = message.avatar();
		const bool normalized_archetype = !avatar_allowed(player.avatar);
		if (normalized_archetype)
			player.avatar.set_archetype_id(
			    m_config.default_avatar_archetype);
		player.avatar.set_revision(player.avatar.revision() + 1);
		*player.profile.mutable_avatar() = player.avatar;
		persist_player(player, now);

		protocol::Envelope accepted;
		if (normalized_archetype)
		{
			auto *fallback = accepted.mutable_avatar_rejected();
			*fallback->mutable_authoritative_avatar() = player.avatar;
			fallback->set_reason(
			    "unknown avatar Soul ID was replaced with the server default");
		}
		else
		{
			accepted.mutable_avatar_accepted()->set_revision(
			    player.avatar.revision());
		}
		queue(
		    *player.connection,
		    std::move(accepted),
		    reliability::reliable);

		protocol::Envelope updated;
		auto *broadcast_update = updated.mutable_player_avatar_updated();
		broadcast_update->set_player_id(player.id);
		*broadcast_update->mutable_avatar() = player.avatar;
		broadcast(
		    std::move(updated),
		    reliability::reliable,
		    player.connection);
	}

	void server_core::handle_chat(
	    player_session &player,
	    const protocol::ChatSend &message,
	    time_point now)
	{
		if (!is_valid_chat(message.text()))
		{
			return;
		}
		const auto cutoff = now - std::chrono::seconds(10);
		while (!player.chat_times.empty() && player.chat_times.front() < cutoff)
		{
			player.chat_times.pop_front();
		}
		if (player.chat_times.size() >= 5)
		{
			return;
		}
		player.chat_times.push_back(now);
		protocol::Envelope envelope;
		auto *chat = envelope.mutable_chat_broadcast();
		chat->set_player_id(player.id);
		chat->set_display_name(player.display_name);
		chat->set_text(message.text());
		chat->set_server_time_ms(milliseconds(now));
		broadcast(std::move(envelope), reliability::reliable);
	}

	void server_core::handle_ping(
	    player_session &player,
	    const protocol::Ping &message,
	    time_point now)
	{
		protocol::Envelope envelope;
		auto *pong = envelope.mutable_pong();
		pong->set_nonce(message.nonce());
		pong->set_client_time_ms(message.client_time_ms());
		pong->set_server_time_ms(milliseconds(now));
		queue(*player.connection, std::move(envelope), reliability::reliable);
	}

	void server_core::reject(
	    connection_id connection,
	    protocol::RejectReason reason,
	    std::string message)
	{
		release_initializer(connection);
		m_pending.erase(connection);
		protocol::Envelope envelope;
		auto *rejected = envelope.mutable_server_rejected();
		rejected->set_reason(reason);
		rejected->set_message(std::move(message));
		queue(
		    connection,
		    std::move(envelope),
		    reliability::reliable,
		    close_kind::reject);
		wake_bootstrap_waiters();
	}

	void server_core::remove_player(
	    player_id id,
	    std::string reason,
	    close_kind close,
	    time_point now)
	{
		const auto iterator = m_players.find(id);
		if (iterator == m_players.end())
		{
			return;
		}
		persist_player(iterator->second, now);
		const auto connection = iterator->second.connection;
		if (connection && close != close_kind::none)
		{
			queue(
			    *connection,
			    player_left_envelope(id, reason),
			    reliability::reliable,
			    close);
		}
		m_players.erase(iterator);
		broadcast(
		    player_left_envelope(id, reason),
		    reliability::reliable,
		    connection);
	}

	void server_core::send_accepted(player_session &player)
	{
		protocol::Envelope envelope;
		auto *accepted = envelope.mutable_server_accepted();
		accepted->set_player_id(player.id);
		accepted->set_resume_token(player.resume_token);
		accepted->set_tick_rate(m_config.tick_rate);
		accepted->set_snapshot_rate(m_config.snapshot_rate);
		accepted->set_max_players(m_config.max_players);
		accepted->set_server_name(m_config.name);
		accepted->set_level_id(m_store.manifest().level_id);
		accepted->set_profile_snapshot_interval_seconds(
		    m_config.profile_snapshot_interval_seconds);
		*accepted->mutable_avatar_policy() = avatar_policy();
		for (const auto &[id, session] : m_players)
		{
			(void)id;
			*accepted->add_players() = snapshot_of(session, true);
		}
		queue(*player.connection, std::move(envelope), reliability::reliable);
		send_entity_control(*player.connection);
	}

	void server_core::send_entity_control(connection_id connection)
	{
		protocol::Envelope envelope;
		envelope.mutable_server_entity_control()
		    ->set_non_player_entities_disabled(
		        m_non_player_entities_disabled);
		queue(connection, std::move(envelope), reliability::reliable);
	}

	void server_core::apply_default_avatar(
	    protocol::PlayerProfile &profile)
	{
		auto *avatar = profile.mutable_avatar();
		avatar->Clear();
		avatar->set_archetype_id(m_config.default_avatar_archetype);
		avatar->set_revision(1);
		avatar->set_stance(protocol::AVATAR_STANCE_RELAXED);
		avatar->set_weapon_class(protocol::AVATAR_WEAPON_CLASS_NONE);
		avatar->set_weapon_drawn(false);
		for (const auto &item : profile.inventory())
		{
			if (!item.has_equipped_slot())
				continue;
			auto *visible = avatar->add_equipment();
			visible->set_definition_id(item.definition_id());
			visible->set_equipped_slot(item.equipped_slot());
		}
	}

	bool server_core::avatar_allowed(
	    const protocol::AvatarDescriptor &avatar) const
	{
		return is_valid_avatar_descriptor(avatar)
		    && std::ranges::find(
		           m_config.allowed_avatar_archetypes,
		           avatar.archetype_id())
		        != m_config.allowed_avatar_archetypes.end();
	}

	protocol::AvatarPolicy server_core::avatar_policy() const
	{
		protocol::AvatarPolicy result;
		result.set_default_archetype_id(
		    m_config.default_avatar_archetype);
		for (const auto &archetype :
		     m_config.allowed_avatar_archetypes)
		{
			result.add_allowed_archetype_ids(archetype);
		}
		return result;
	}

	void server_core::send_challenge(connection_id connection)
	{
		protocol::Envelope envelope;
		envelope.mutable_server_challenge()->set_server_id(
		    m_store.manifest().server_id);
		queue(connection, std::move(envelope), reliability::reliable);
	}

	void server_core::send_bootstrap(
	    connection_id connection,
	    protocol::BootstrapMode mode)
	{
		auto &pending = m_pending.at(connection);
		protocol::Envelope envelope;
		auto *bootstrap = envelope.mutable_server_bootstrap();
		bootstrap->set_server_id(m_store.manifest().server_id);
		bootstrap->set_session_id(m_store.manifest().session_id);
		bootstrap->set_manifest_revision(m_store.manifest().revision);
		bootstrap->set_level_id(m_store.manifest().level_id);
		bootstrap->set_world_seed(m_store.manifest().world_seed);
		bootstrap->set_mode(mode);
		bootstrap->set_spawn_valid(m_store.manifest().spawn_valid);
		bootstrap->set_timeout_seconds(m_config.bootstrap_timeout_seconds);
		bootstrap->set_issued_identity_token(
		    pending.issued_identity_token);
		if (m_store.manifest().spawn_valid)
		{
			*bootstrap->mutable_spawn() = m_store.manifest().spawn;
		}
		if (pending.persisted)
		{
			*bootstrap->mutable_profile() = pending.persisted->profile;
		}
		queue(connection, std::move(envelope), reliability::reliable);
	}

	void server_core::release_initializer(connection_id connection)
	{
		if (m_initializer == connection)
		{
			m_initializer.reset();
		}
	}

	void server_core::wake_bootstrap_waiters()
	{
		if (m_store.manifest().spawn_valid)
		{
			for (auto &[connection, pending] : m_pending)
			{
				if (pending.stage == pending_stage::waiting_for_initializer)
				{
					pending.stage = pending_stage::loading_world;
					send_bootstrap(connection, protocol::BOOTSTRAP_MODE_LOAD);
				}
			}
			return;
		}
		if (m_initializer)
		{
			return;
		}
		const auto waiter = std::ranges::find_if(
		    m_pending,
		    [](const auto &entry)
		    {
			    return entry.second.stage
			        == pending_stage::waiting_for_initializer;
		    });
		if (waiter != m_pending.end())
		{
			m_initializer = waiter->first;
			waiter->second.initializer = true;
			waiter->second.stage = pending_stage::loading_world;
			send_bootstrap(
			    waiter->first,
			    protocol::BOOTSTRAP_MODE_INITIALIZE);
		}
	}

	void server_core::persist_player(player_session &player, time_point now)
	{
		if (player.dummy)
		{
			return;
		}
		player.profile.set_player_id(player.id);
		player.profile.set_display_name(player.display_name);
		player.profile.set_level_id(m_store.manifest().level_id);
		player.profile.set_transform_valid(player.has_transform);
		if (player.has_transform)
		{
			*player.profile.mutable_last_transform() = player.transform;
		}
		*player.profile.mutable_avatar() = player.avatar;
		m_store.save_profile(player.identity_hash, player.profile);
		player.last_persisted_at = now;
	}

	void server_core::broadcast(
	    protocol::Envelope envelope,
	    reliability delivery,
	    std::optional<connection_id> except)
	{
		for (const auto &[id, player] : m_players)
		{
			(void)id;
			if (!player.connection || player.connection == except)
			{
				continue;
			}
			queue(*player.connection, envelope, delivery);
		}
	}

	void server_core::queue(
	    connection_id connection,
	    protocol::Envelope envelope,
	    reliability delivery,
	    close_kind close)
	{
		m_outbound.push_back(
		    {connection, std::move(envelope), delivery, close});
	}

	void server_core::queue_snapshot(time_point now)
	{
		if (m_players.empty())
		{
			return;
		}
		protocol::Envelope envelope;
		auto *snapshot = envelope.mutable_world_snapshot();
		snapshot->set_server_tick(m_server_tick);
		snapshot->set_server_time_ms(milliseconds(now));
		for (const auto &[id, player] : m_players)
		{
			(void)id;
			*snapshot->add_players() = snapshot_of(player, false);
		}
		broadcast(std::move(envelope), reliability::unreliable);
	}

	server_core::player_session *server_core::find_by_connection(
	    connection_id connection)
	{
		for (auto &[id, player] : m_players)
		{
			(void)id;
			if (player.connection == connection)
			{
				return &player;
			}
		}
		return nullptr;
	}

	server_core::player_session *server_core::find_by_resume_token(
	    std::string_view token)
	{
		for (auto &[id, player] : m_players)
		{
			(void)id;
			if (player.resume_token == token)
			{
				return &player;
			}
		}
		return nullptr;
	}

	std::string server_core::lower_ascii(std::string_view value)
	{
		return lowercase_ascii(value);
	}

	std::uint64_t server_core::milliseconds(time_point value)
	{
		return static_cast<std::uint64_t>(
		    std::chrono::duration_cast<std::chrono::milliseconds>(
		        value.time_since_epoch())
		        .count());
	}

	protocol::PlayerSnapshot server_core::snapshot_of(
	    const player_session &player,
	    bool include_avatar)
	{
		protocol::PlayerSnapshot snapshot;
		snapshot.set_player_id(player.id);
		snapshot.set_display_name(player.display_name);
		snapshot.set_transform_valid(player.has_transform);
		snapshot.set_connected(
		    player.dummy || player.connection.has_value());
		snapshot.set_movement_mode(player.movement_mode);
		if (player.has_transform)
		{
			*snapshot.mutable_transform() = player.transform;
		}
		if (include_avatar)
		{
			*snapshot.mutable_avatar() = player.avatar;
		}
		return snapshot;
	}
}
