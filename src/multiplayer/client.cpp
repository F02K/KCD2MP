#include "multiplayer/client.hpp"
#include "multiplayer/avatar_visual.hpp"
#include "multiplayer/game_bridge.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <type_traits>

namespace kcd2mp
{
	namespace
	{
		constexpr std::array reconnect_delays{
		    std::chrono::seconds(1),
		    std::chrono::seconds(2),
		    std::chrono::seconds(4),
		    std::chrono::seconds(8),
		    std::chrono::seconds(8)};

		std::uint64_t milliseconds(std::chrono::steady_clock::time_point value)
		{
			return static_cast<std::uint64_t>(
			    std::chrono::duration_cast<std::chrono::milliseconds>(
			        value.time_since_epoch())
			        .count());
		}

		float distance(
		    const protocol::Vec3 &left,
		    const protocol::Vec3 &right)
		{
			return std::sqrt(
			    std::pow(left.x() - right.x(), 2.0F)
			    + std::pow(left.y() - right.y(), 2.0F)
			    + std::pow(left.z() - right.z(), 2.0F));
		}

	}

	multiplayer_client::multiplayer_client() :
	    m_network_thread(
	        [this](std::stop_token stop)
	        {
		        network_loop(stop);
	        })
	{
	}

	multiplayer_client::~multiplayer_client()
	{
		m_network_thread.request_stop();
		queue_network(disconnect_command{});
		if (m_network_thread.joinable())
		{
			m_network_thread.join();
		}
	}

	bool multiplayer_client::connect(client_options options)
	{
		if (!is_valid_display_name(options.display_name)
		    || options.address.empty())
		{
			return false;
		}
		const auto sandbox = game::sandbox_capability();
		if (!sandbox.available || !game::can_start_join())
		{
			std::scoped_lock lock(m_state_mutex);
			m_status.error = !sandbox.available
			    ? sandbox.diagnostic
			    : "Join requires a fully loaded native save.";
			return false;
		}
		{
			std::scoped_lock lock(m_state_mutex);
			if (m_status.state != client_state::disconnected)
			{
				return false;
			}
			m_status = {};
			m_status.state = client_state::preflight;
			m_remote_players.clear();
			m_chat.clear();
			m_local_correction.reset();
			m_profile.reset();
			m_local_avatar.reset();
			m_pending_avatar.reset();
			m_desired_avatar.reset();
			m_desired_archetype.reset();
			m_pending_bootstrap.reset();
			m_profile_update_pending = false;
			m_avatar_update_pending = false;
			m_last_avatar_sent = {};
			m_profile_snapshot_interval_seconds = 15;
			m_resume_token.clear();
		}
		queue_network(connect_command{std::move(options)});
		return true;
	}

	void multiplayer_client::disconnect()
	{
		if (const auto profile = game::local_profile())
		{
			queue_profile_snapshot(*profile);
		}
		{
			std::scoped_lock lock(m_state_mutex);
			if (m_status.state == client_state::disconnected)
			{
				return;
			}
			m_status.state = client_state::closing;
		}
		queue_network(disconnect_command{});
	}

	void multiplayer_client::fail(std::string error)
	{
		{
			std::scoped_lock lock(m_state_mutex);
			if (m_status.state == client_state::disconnected
			    || m_status.state == client_state::closing)
			{
				return;
			}
			m_status.state = client_state::closing;
			m_status.error = std::move(error);
		}
		queue_network(disconnect_command{});
	}

	bool multiplayer_client::send_chat(std::string text)
	{
		if (!is_valid_chat(text))
		{
			return false;
		}
		{
			std::scoped_lock lock(m_state_mutex);
			if (m_status.state != client_state::connected)
			{
				return false;
			}
		}
		queue_network(chat_command{std::move(text)});
		return true;
	}

	bool multiplayer_client::select_avatar(std::string archetype_id)
	{
		std::scoped_lock lock(m_state_mutex);
		if (m_status.state != client_state::connected
		    || !m_local_avatar
		    || std::ranges::find(
		           m_status.avatar_policy.allowed_archetype_ids(),
		           archetype_id)
		        == m_status.avatar_policy.allowed_archetype_ids().end())
		{
			return false;
		}
		m_desired_archetype = std::move(archetype_id);
		return true;
	}

	void multiplayer_client::game_tick(
	    std::optional<protocol::TransformState> local_transform,
	    std::optional<protocol::AvatarDescriptor> local_avatar_visual,
	    std::string_view current_level,
	    std::chrono::steady_clock::time_point now)
	{
		for (const auto &envelope : m_game_commands.drain())
		{
			handle_game_envelope(envelope, now);
		}
		advance_sandbox_bootstrap();

		bool connected = false;
		bool profile_due = false;
		std::optional<protocol::ClientAvatarUpdate> avatar_update;
		std::string expected_level;
		{
			std::scoped_lock lock(m_state_mutex);
			connected = m_status.state == client_state::connected;
			expected_level = m_status.level_id;
			profile_due = connected && !m_profile_update_pending
			    && (m_last_profile_sent
			            == std::chrono::steady_clock::time_point{}
			        || now - m_last_profile_sent
			            >= std::chrono::seconds(
			                m_profile_snapshot_interval_seconds));
			m_status.game_queue_size = m_game_commands.size();
			if (connected && m_local_avatar)
			{
				auto desired = merge_avatar_visual(
				    *m_local_avatar,
				    local_avatar_visual,
				    m_desired_archetype);
				m_desired_avatar = desired;
				const bool rate_due =
				    m_last_avatar_sent
				            == std::chrono::steady_clock::time_point{}
				    || now - m_last_avatar_sent
				        >= std::chrono::milliseconds(250);
				if (!m_avatar_update_pending && rate_due
				    && !same_avatar_visual(desired, *m_local_avatar))
				{
					protocol::ClientAvatarUpdate update;
					update.set_base_revision(m_local_avatar->revision());
					*update.mutable_avatar() = desired;
					update.mutable_avatar()->set_revision(
					    update.base_revision());
					m_pending_avatar = update.avatar();
					m_avatar_update_pending = true;
					m_last_avatar_sent = now;
					avatar_update = std::move(update);
				}
			}
		}
		if (avatar_update)
			queue_network(avatar_command{std::move(*avatar_update)});
		if (connected && current_level != expected_level)
		{
			set_state(
			    client_state::disconnected,
			    "loaded level no longer matches the server");
			queue_network(disconnect_command{});
			return;
		}
		if (connected && local_transform
		    && (m_last_transform_sent == std::chrono::steady_clock::time_point{}
		        || now - m_last_transform_sent >= std::chrono::milliseconds(33)))
		{
			queue_network(transform_command{std::move(*local_transform)});
			m_last_transform_sent = now;
		}
		if (profile_due)
		{
			if (const auto profile = game::local_profile())
			{
				queue_profile_snapshot(*profile);
			}
		}
		update_interpolation(now);
	}

	client_status multiplayer_client::status() const
	{
		std::scoped_lock lock(m_state_mutex);
		return m_status;
	}

	std::vector<remote_player_view> multiplayer_client::remote_players() const
	{
		std::scoped_lock lock(m_state_mutex);
		std::vector<remote_player_view> result;
		result.reserve(m_remote_players.size());
		for (const auto &[id, player] : m_remote_players)
		{
			(void)id;
			result.push_back(player.rendered);
		}
		std::ranges::sort(result, {}, &remote_player_view::id);
		return result;
	}

	std::vector<chat_entry> multiplayer_client::chat_history() const
	{
		std::scoped_lock lock(m_state_mutex);
		return {m_chat.begin(), m_chat.end()};
	}

	std::optional<protocol::TransformState>
	multiplayer_client::take_local_correction()
	{
		std::scoped_lock lock(m_state_mutex);
		auto correction = std::move(m_local_correction);
		m_local_correction.reset();
		return correction;
	}

	void multiplayer_client::network_loop(std::stop_token stop)
	{
		using namespace std::chrono_literals;
		try
		{
			net::runtime runtime;
			std::optional<net::client_transport> transport;
			bool transport_needs_reset = false;
			client_options options;
			std::size_t reconnect_attempt = 0;
			auto reconnect_at = std::chrono::steady_clock::time_point{};
			auto last_ping = std::chrono::steady_clock::time_point{};

			auto send_envelope =
			    [&](const protocol::Envelope &envelope, reliability delivery)
			{
				if (!transport || !transport->has_connection())
				{
					return false;
				}
				std::string error;
				const auto encoded = encode(envelope, delivery, &error);
				return encoded
				    && transport->send(encoded->bytes, delivery, &error);
			};

			auto create_transport = [&]
			{
				transport.emplace(net::client_callbacks{
				    .connected =
				        [&]
				        {
					        set_state(client_state::preflight);
					        protocol::Envelope envelope;
					        auto *hello = envelope.mutable_client_hello();
					        hello->set_protocol_version(protocol_version);
					        hello->set_client_version(version_string);
					        hello->set_whgame_timestamp(supported_whgame_timestamp);
					        hello->set_whgame_image_size(
					            supported_whgame_image_size);
					        hello->set_display_name(options.display_name);
					        hello->set_password(options.password);
					        hello->set_content_hash(options.content_hash);
					        if (!send_envelope(envelope, reliability::reliable))
					        {
						        set_state(
						            client_state::disconnected,
						            "failed to send ClientHello");
					        }
				        },
				    .disconnected =
				        [&](bool retry, std::string reason)
				        {
					        transport_needs_reset = true;
					        if (retry
					            && reconnect_attempt < reconnect_delays.size())
					        {
						        reconnect_at = std::chrono::steady_clock::now()
						            + reconnect_delays[reconnect_attempt++];
						        set_state(client_state::reconnecting, reason);
					        }
					        else
					        {
						        set_state(client_state::disconnected, reason);
					        }
				        },
				    .message =
				        [&](std::span<const std::byte> bytes)
				        {
					        std::string error;
					        const auto envelope = decode(bytes, &error);
					        if (!envelope)
					        {
						        set_state(
						            client_state::disconnected,
						            "server sent malformed data");
						        if (transport)
						        {
							        transport->abort_connection(
							            "malformed server message");
						        }
						        return;
					        }
					        if (envelope->has_server_accepted())
					        {
						        const auto &accepted =
						            envelope->server_accepted();
						        {
							        std::scoped_lock lock(m_state_mutex);
							        m_status.state = client_state::connected;
							        m_status.local_player_id =
							            accepted.player_id();
							        m_status.server_name =
							            accepted.server_name();
							        m_status.level_id = accepted.level_id();
							        m_status.error.clear();
							        m_profile_snapshot_interval_seconds =
							            accepted
							                .profile_snapshot_interval_seconds();
							        m_resume_token = accepted.resume_token();
						        }
						        reconnect_attempt = 0;
					        }
					        else if (envelope->has_server_challenge())
					        {
						        const auto server_id =
						            envelope->server_challenge().server_id();
						        {
							        std::scoped_lock lock(m_state_mutex);
							        m_server_id = server_id;
							        m_status.server_id = server_id;
							        m_status.state = client_state::authenticating;
						        }
						        protocol::Envelope authentication;
						        auto *message =
						            authentication.mutable_client_authenticate();
						        if (!options.claim_code.empty())
						        {
							        message->set_claim_code(options.claim_code);
						        }
						        else if (const auto token =
						                     m_identities.token_for(server_id))
						        {
							        message->set_identity_token(*token);
						        }
						        else
						        {
							        message->set_enroll(true);
						        }
						        {
							        std::scoped_lock lock(m_state_mutex);
							        message->set_resume_token(m_resume_token);
						        }
						        if (!send_envelope(
						                authentication,
						                reliability::reliable))
						        {
							        set_state(
							            client_state::disconnected,
							            "failed to send ClientAuthenticate");
						        }
					        }
					        else if (envelope->has_server_bootstrap())
					        {
						        const auto &bootstrap =
						            envelope->server_bootstrap();
						        if (!bootstrap.issued_identity_token().empty())
						        {
							        m_identities.store(
							            bootstrap.server_id(),
							            bootstrap.issued_identity_token());
						        }
						        {
							        std::scoped_lock lock(m_state_mutex);
							        m_status.server_id = bootstrap.server_id();
							        m_status.session_id = bootstrap.session_id();
							        m_status.level_id = bootstrap.level_id();
							        m_status.state =
							            bootstrap.mode()
							                == protocol::BOOTSTRAP_MODE_WAIT
							            ? client_state::waiting_for_bootstrap
							            : client_state::loading_sandbox;
						        }
					        }
					        else if (envelope->has_server_rejected())
					        {
						        set_state(
						            client_state::disconnected,
						            envelope->server_rejected().message());
						        if (transport)
						        {
							        transport->abort_connection(
							            "server rejected connection");
						        }
					        }

					        const bool reliable =
					            !envelope->has_world_snapshot();
					        if (!m_game_commands.push(
					                std::move(*envelope),
					                reliable)
					            && reliable)
					        {
						        set_state(
						            client_state::disconnected,
						            "game-thread queue overflow");
						        if (transport)
						        {
							        transport->abort_connection(
							            "client queue overflow");
						        }
					        }
				        }});
			};

			while (!stop.stop_requested())
			{
				std::deque<network_command> commands;
				{
					std::scoped_lock lock(m_network_mutex);
					commands.swap(m_network_commands);
				}
				for (auto &command : commands)
				{
					std::visit(
					    [&](auto &typed)
					    {
						    using type = std::decay_t<decltype(typed)>;
						    if constexpr (std::is_same_v<type, connect_command>)
						    {
							    options = std::move(typed.options);
							    reconnect_attempt = 0;
							    create_transport();
							    transport->connect(options.address);
						    }
						    else if constexpr (
						        std::is_same_v<type, disconnect_command>)
						    {
							    if (transport)
							    {
								    transport->disconnect();
								    transport.reset();
							    }
							    set_state(client_state::disconnected);
						    }
						    else if constexpr (
						        std::is_same_v<type, transform_command>)
						    {
							    protocol::Envelope envelope;
							    *envelope.mutable_client_transform()
							         ->mutable_transform() =
							        std::move(typed.transform);
							    (void)send_envelope(
							        envelope,
							        reliability::unreliable);
						    }
						    else if constexpr (
						        std::is_same_v<type, chat_command>)
						    {
							    protocol::Envelope envelope;
							    envelope.mutable_chat_send()->set_text(
							        std::move(typed.text));
							    (void)send_envelope(
							        envelope,
							        reliability::reliable);
						    }
						    else if constexpr (
						        std::is_same_v<type, world_ready_command>)
						    {
							    protocol::Envelope envelope;
							    *envelope.mutable_client_world_ready() =
							        std::move(typed.message);
							    (void)send_envelope(
							        envelope,
							        reliability::reliable);
						    }
						    else if constexpr (
						        std::is_same_v<type, world_failed_command>)
						    {
							    protocol::Envelope envelope;
							    *envelope.mutable_client_world_failed() =
							        std::move(typed.message);
							    (void)send_envelope(
							        envelope,
							        reliability::reliable);
						    }
						    else if constexpr (
						        std::is_same_v<type, profile_command>)
						    {
							    protocol::Envelope envelope;
							    *envelope.mutable_client_profile_update() =
							        std::move(typed.message);
							    (void)send_envelope(
							        envelope,
							        reliability::reliable);
						    }
						    else if constexpr (
						        std::is_same_v<type, avatar_command>)
						    {
							    protocol::Envelope envelope;
							    *envelope.mutable_client_avatar_update() =
							        std::move(typed.message);
							    (void)send_envelope(
							        envelope,
							        reliability::reliable);
						    }
					    },
					    command);
				}

				if (!transport
				    && reconnect_at != std::chrono::steady_clock::time_point{}
				    && std::chrono::steady_clock::now() >= reconnect_at)
				{
					create_transport();
					transport->connect(options.address);
					reconnect_at = {};
				}
				if (transport)
				{
					transport->poll();
					if (transport_needs_reset)
					{
						transport.reset();
						transport_needs_reset = false;
						continue;
					}
					const auto current = std::chrono::steady_clock::now();
					if (transport && transport->has_connection()
					    && (last_ping == std::chrono::steady_clock::time_point{}
					        || current - last_ping >= 3s))
					{
						protocol::Envelope ping;
						ping.mutable_ping()->set_nonce(milliseconds(current));
						ping.mutable_ping()->set_client_time_ms(
						    milliseconds(current));
						(void)send_envelope(ping, reliability::reliable);
						last_ping = current;
					}
					if (transport)
					{
						std::scoped_lock lock(m_state_mutex);
						m_status.ping_ms = transport->ping_ms();
						m_status.packet_loss_percent =
						    transport->packet_loss_percent();
					}
				}
				std::this_thread::sleep_for(1ms);
			}
			if (transport)
			{
				transport->disconnect("KCD2MP client shutting down");
			}
		}
		catch (const std::exception &exception)
		{
			set_state(client_state::disconnected, exception.what());
		}
	}

	void multiplayer_client::set_state(
	    client_state state,
	    std::string error)
	{
		std::scoped_lock lock(m_state_mutex);
		m_status.state = state;
		m_status.error = std::move(error);
		if (state == client_state::disconnected)
		{
			m_pending_bootstrap.reset();
			m_status.local_player_id = 0;
			m_status.ping_ms = -1;
			m_status.packet_loss_percent = 0.0F;
		}
	}

	void multiplayer_client::queue_network(network_command command)
	{
		std::scoped_lock lock(m_network_mutex);
		m_network_commands.push_back(std::move(command));
	}

	void multiplayer_client::queue_profile_snapshot(
	    protocol::PlayerProfile profile)
	{
		protocol::ClientProfileUpdate update;
		{
			std::scoped_lock lock(m_state_mutex);
			if (m_status.state != client_state::connected || !m_profile
			    || m_profile_update_pending)
			{
				return;
			}
			profile.set_player_id(m_profile->player_id());
			profile.set_revision(m_profile->revision());
			profile.set_display_name(m_profile->display_name());
			profile.set_level_id(m_profile->level_id());
			if (!is_valid_profile(profile))
			{
				m_status.error =
				    "native profile capture returned an invalid profile";
				return;
			}
			update.set_base_revision(m_profile->revision());
			*update.mutable_profile() = std::move(profile);
			m_profile_update_pending = true;
			m_last_profile_sent = std::chrono::steady_clock::now();
		}
		queue_network(profile_command{std::move(update)});
	}

	void multiplayer_client::handle_game_envelope(
	    const protocol::Envelope &envelope,
	    std::chrono::steady_clock::time_point now)
	{
		std::unique_lock lock(m_state_mutex);
		if (envelope.has_server_accepted())
		{
			m_status.avatar_policy =
			    envelope.server_accepted().avatar_policy();
			for (const auto &player : envelope.server_accepted().players())
			{
				accept_snapshot_player(player, now);
			}
		}
		else if (envelope.has_server_bootstrap())
		{
			const auto &bootstrap = envelope.server_bootstrap();
			if (m_status.state == client_state::disconnected
			    || m_status.state == client_state::closing)
			{
				return;
			}
			if (bootstrap.mode() == protocol::BOOTSTRAP_MODE_WAIT)
			{
				m_status.state = client_state::waiting_for_bootstrap;
				return;
			}
			m_status.state = client_state::loading_sandbox;
			if (!bootstrap.has_profile())
			{
				m_status.error = "server bootstrap did not include a player profile";
				protocol::ClientWorldFailed failed;
				failed.set_session_id(bootstrap.session_id());
				failed.set_reason(m_status.error);
				queue_network(world_failed_command{std::move(failed)});
				return;
			}
			const auto bootstrap_copy = bootstrap;
			lock.unlock();
			const auto result = game::begin_sandbox(bootstrap_copy);
			lock.lock();
			if (!result.started)
			{
				protocol::ClientWorldFailed failed;
				failed.set_session_id(bootstrap.session_id());
				failed.set_reason(result.error);
				queue_network(world_failed_command{std::move(failed)});
				m_status.error = result.error;
				return;
			}
			if (m_status.state == client_state::disconnected
			    || m_status.state == client_state::closing)
			{
				lock.unlock();
				game::end_sandbox();
				return;
			}
			m_pending_bootstrap = bootstrap_copy;
		}
		else if (envelope.has_player_joined())
		{
			accept_snapshot_player(envelope.player_joined().player(), now);
		}
		else if (envelope.has_player_left())
		{
			m_remote_players.erase(envelope.player_left().player_id());
		}
		else if (envelope.has_world_snapshot())
		{
			for (const auto &player : envelope.world_snapshot().players())
			{
				accept_snapshot_player(player, now);
			}
		}
		else if (envelope.has_state_correction())
		{
			m_local_correction =
			    envelope.state_correction().accepted_transform();
		}
		else if (envelope.has_profile_accepted())
		{
			if (!m_profile || !m_profile_update_pending
			    || envelope.profile_accepted().revision()
			        != m_profile->revision() + 1)
			{
				m_status.state = client_state::closing;
				m_status.error = "server returned an invalid profile revision";
				queue_network(disconnect_command{});
				return;
			}
			m_profile->set_revision(
			    envelope.profile_accepted().revision());
			m_profile_update_pending = false;
		}
		else if (envelope.has_profile_rejected())
		{
			m_profile_update_pending = false;
			m_status.state = client_state::closing;
			m_status.error = envelope.profile_rejected().reason();
			queue_network(disconnect_command{});
		}
		else if (envelope.has_avatar_accepted())
		{
			if (!m_local_avatar || !m_pending_avatar
			    || !m_avatar_update_pending
			    || envelope.avatar_accepted().revision()
			        != m_local_avatar->revision() + 1)
			{
				m_status.state = client_state::closing;
				m_status.error =
				    "server returned an invalid avatar revision";
				queue_network(disconnect_command{});
				return;
			}
			m_pending_avatar->set_revision(
			    envelope.avatar_accepted().revision());
			m_local_avatar = *m_pending_avatar;
			m_status.avatar_archetype_id =
			    m_local_avatar->archetype_id();
			m_pending_avatar.reset();
			m_desired_archetype.reset();
			if (m_profile)
			{
				*m_profile->mutable_avatar() = *m_local_avatar;
			}
			m_avatar_update_pending = false;
		}
		else if (envelope.has_avatar_rejected())
		{
			m_local_avatar =
			    envelope.avatar_rejected().authoritative_avatar();
			m_status.avatar_archetype_id =
			    m_local_avatar->archetype_id();
			m_pending_avatar.reset();
			m_desired_archetype.reset();
			if (m_profile)
			{
				*m_profile->mutable_avatar() = *m_local_avatar;
			}
			m_avatar_update_pending = false;
			m_status.error = envelope.avatar_rejected().reason();
		}
		else if (envelope.has_player_avatar_updated())
		{
			const auto &message = envelope.player_avatar_updated();
			if (message.player_id() != m_status.local_player_id)
			{
				auto &remote = m_remote_players[message.player_id()];
				remote.rendered.id = message.player_id();
				remote.rendered.avatar = message.avatar();
				remote.rendered.has_avatar = true;
			}
		}
		else if (envelope.has_chat_broadcast())
		{
			const auto &message = envelope.chat_broadcast();
			m_chat.push_back({
			    message.player_id(),
			    message.display_name(),
			    message.text(),
			    message.server_time_ms()});
			while (m_chat.size() > 200)
			{
				m_chat.pop_front();
			}
		}
		else if (envelope.has_server_entity_control())
		{
			const bool disabled =
			    envelope.server_entity_control()
			        .non_player_entities_disabled();
			lock.unlock();
			const bool applied =
			    game::set_non_player_entities_disabled(disabled);
			lock.lock();
			if (!applied)
			{
				m_status.state = client_state::closing;
				m_status.error =
				    "could not apply the server's entity-control state";
				queue_network(disconnect_command{});
			}
		}
		else if (envelope.has_server_shutdown())
		{
			m_status.state = client_state::disconnected;
			m_status.error = envelope.server_shutdown().reason();
			m_remote_players.clear();
		}
	}

	void multiplayer_client::advance_sandbox_bootstrap()
	{
		std::optional<protocol::ServerBootstrap> bootstrap;
		{
			std::scoped_lock lock(m_state_mutex);
			if (!m_pending_bootstrap)
			{
				return;
			}
			bootstrap = *m_pending_bootstrap;
		}

		const auto progress = game::poll_sandbox();
		if (progress.phase == game::sandbox_phase::loading)
		{
			return;
		}
		if (progress.phase != game::sandbox_phase::ready)
		{
			const auto reason = progress.error.empty()
			    ? "sandbox bootstrap stopped before the world became ready"
			    : progress.error;
			protocol::ClientWorldFailed failed;
			failed.set_session_id(bootstrap->session_id());
			failed.set_reason(reason);
			{
				std::scoped_lock lock(m_state_mutex);
				if (!m_pending_bootstrap
				    || m_pending_bootstrap->session_id()
				        != bootstrap->session_id())
				{
					return;
				}
				m_pending_bootstrap.reset();
				m_status.state = client_state::closing;
				m_status.error = reason;
			}
			queue_network(world_failed_command{std::move(failed)});
			game::end_sandbox();
			return;
		}

		protocol::ClientWorldReady ready;
		ready.set_session_id(bootstrap->session_id());
		ready.set_manifest_revision(bootstrap->manifest_revision());
		ready.set_level_id(bootstrap->level_id());
		ready.set_initialized_session(
		    bootstrap->mode() == protocol::BOOTSTRAP_MODE_INITIALIZE);
		if (progress.initial_spawn)
		{
			*ready.mutable_initial_spawn() = *progress.initial_spawn;
		}
		if (!bootstrap->profile().has_avatar())
		{
			protocol::ClientWorldFailed failed;
			failed.set_session_id(bootstrap->session_id());
			failed.set_reason(
			    "server profile has no avatar descriptor");
			queue_network(world_failed_command{std::move(failed)});
			return;
		}
		*ready.mutable_avatar() = bootstrap->profile().avatar();
		{
			std::scoped_lock lock(m_state_mutex);
			if (!m_pending_bootstrap
			    || m_pending_bootstrap->session_id()
			        != bootstrap->session_id())
			{
				return;
			}
			m_pending_bootstrap.reset();
			m_status.state = client_state::applying_profile;
			m_profile = bootstrap->profile();
			m_local_avatar = bootstrap->profile().avatar();
			m_status.avatar_archetype_id =
			    m_local_avatar->archetype_id();
			m_pending_avatar.reset();
			m_desired_avatar.reset();
			m_desired_archetype.reset();
			m_profile_update_pending = false;
			m_avatar_update_pending = false;
		}
		queue_network(world_ready_command{std::move(ready)});
	}

	void multiplayer_client::update_interpolation(
	    std::chrono::steady_clock::time_point now)
	{
		const auto target = now - std::chrono::milliseconds(100);
		std::scoped_lock lock(m_state_mutex);
		for (auto &[id, player] : m_remote_players)
		{
			(void)id;
			if (player.history.empty())
			{
				continue;
			}
			while (player.history.size() > 2
			    && player.history[1].received_at <= target)
			{
				player.history.pop_front();
			}

			auto rendered = player.history.front().transform;
			auto mode = player.history.front().mode;
			bool connected = player.history.front().connected;
			if (player.history.size() >= 2
			    && player.history.front().received_at <= target)
			{
				const auto &from = player.history[0];
				const auto &to = player.history[1];
				const auto duration =
				    std::chrono::duration<float>(to.received_at - from.received_at)
				        .count();
				const auto elapsed =
				    std::chrono::duration<float>(target - from.received_at).count();
				const auto factor = duration <= 0.0F
				    ? 1.0F
				    : std::clamp(elapsed / duration, 0.0F, 1.0F);
				rendered = interpolate(from.transform, to.transform, factor);
				mode = factor < 0.5F ? from.mode : to.mode;
				connected = to.connected;
			}
			else if (player.history.size() == 1
			    && target > player.history.front().received_at)
			{
				const auto seconds = std::min(
				    std::chrono::duration<float>(
				        target - player.history.front().received_at)
				        .count(),
				    0.25F);
				rendered = extrapolate(player.history.front().transform, seconds);
			}

			if (player.rendered.has_transform
			    && distance(
			           player.rendered.transform.position(),
			           rendered.position())
			        > 5.0F)
			{
				player.rendered.transform = rendered;
			}
			else
			{
				player.rendered.transform = std::move(rendered);
			}
			player.rendered.has_transform = true;
			player.rendered.movement_mode = mode;
			player.rendered.connected = connected;
		}
	}

	void multiplayer_client::accept_snapshot_player(
	    const protocol::PlayerSnapshot &snapshot,
	    std::chrono::steady_clock::time_point now)
	{
		if (snapshot.player_id() == m_status.local_player_id)
		{
			return;
		}
		auto &player = m_remote_players[snapshot.player_id()];
		player.display_name = snapshot.display_name();
		player.rendered.id = snapshot.player_id();
		player.rendered.display_name = snapshot.display_name();
		player.rendered.connected = snapshot.connected();
		if (snapshot.has_avatar())
		{
			player.rendered.avatar = snapshot.avatar();
			player.rendered.has_avatar = true;
		}
		if (!snapshot.transform_valid() || !snapshot.has_transform())
		{
			return;
		}
		player.history.push_back({
		    now,
		    snapshot.transform(),
		    snapshot.movement_mode(),
		    snapshot.connected()});
		while (player.history.size() > 32)
		{
			player.history.pop_front();
		}
	}

	protocol::TransformState multiplayer_client::interpolate(
	    const protocol::TransformState &from,
	    const protocol::TransformState &to,
	    float factor)
	{
		protocol::TransformState result = to;
		auto lerp = [factor](float left, float right)
		{
			return left + (right - left) * factor;
		};
		result.mutable_position()->set_x(
		    lerp(from.position().x(), to.position().x()));
		result.mutable_position()->set_y(
		    lerp(from.position().y(), to.position().y()));
		result.mutable_position()->set_z(
		    lerp(from.position().z(), to.position().z()));
		result.mutable_velocity()->set_x(
		    lerp(from.velocity().x(), to.velocity().x()));
		result.mutable_velocity()->set_y(
		    lerp(from.velocity().y(), to.velocity().y()));
		result.mutable_velocity()->set_z(
		    lerp(from.velocity().z(), to.velocity().z()));
		auto *rotation = result.mutable_rotation();
		const auto dot = from.rotation().x() * to.rotation().x()
		    + from.rotation().y() * to.rotation().y()
		    + from.rotation().z() * to.rotation().z()
		    + from.rotation().w() * to.rotation().w();
		const auto sign = dot < 0.0F ? -1.0F : 1.0F;
		rotation->set_x(
		    lerp(from.rotation().x(), to.rotation().x() * sign));
		rotation->set_y(
		    lerp(from.rotation().y(), to.rotation().y() * sign));
		rotation->set_z(
		    lerp(from.rotation().z(), to.rotation().z() * sign));
		rotation->set_w(
		    lerp(from.rotation().w(), to.rotation().w() * sign));
		(void)normalize_rotation(rotation);
		return result;
	}

	protocol::TransformState multiplayer_client::extrapolate(
	    const protocol::TransformState &from,
	    float seconds)
	{
		auto result = from;
		result.mutable_position()->set_x(
		    from.position().x() + from.velocity().x() * seconds);
		result.mutable_position()->set_y(
		    from.position().y() + from.velocity().y() * seconds);
		result.mutable_position()->set_z(
		    from.position().z() + from.velocity().z() * seconds);
		return result;
	}

	const char *to_string(client_state state)
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
