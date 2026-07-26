#include "server/server_core.hpp"

#include <Windows.h>

#include <cassert>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <exception>
#include <string>

#undef assert
#define assert(expression)                                                   \
	do                                                                       \
	{                                                                        \
		if (!(expression))                                                   \
		{                                                                    \
			std::cerr << "assertion failed at " << __FILE__ << ':' << __LINE__ \
			          << ": " #expression << '\n';                          \
			std::_Exit(1);                                                   \
		}                                                                    \
	} while (false)

namespace
{
	using namespace std::chrono_literals;
	using namespace kcd2mp;
	using namespace kcd2mp::server;

	struct temporary_world
	{
		temporary_world()
		{
			static std::uint32_t next_id{};
			path = std::filesystem::temp_directory_path()
			    / ("kcd2mp-server-tests-"
			        + std::to_string(GetCurrentProcessId()) + "-"
			        + std::to_string(GetTickCount64()) + "-"
			        + std::to_string(++next_id));
			std::filesystem::create_directories(path);
		}

		~temporary_world()
		{
			std::error_code ignored;
			std::filesystem::remove_all(path, ignored);
		}

		std::filesystem::path path;
	};

	server_config config_for(
	    const std::filesystem::path &world,
	    bool configured_spawn = true)
	{
		server_config config;
		config.name = "Sandbox Test";
		config.password = "secret";
		config.level_id = "sandbox";
		config.world_directory = world;
		if (configured_spawn)
		{
			config.initial_spawn = initial_spawn_config{
			    10.0F, 20.0F, 30.0F, 0.0F, 0.0F, 0.0F, 1.0F};
		}
		return config;
	}

	protocol::Envelope hello(std::string name = "Henry")
	{
		protocol::Envelope envelope;
		auto *message = envelope.mutable_client_hello();
		message->set_protocol_version(protocol_version);
		message->set_client_version(version_string);
		message->set_whgame_timestamp(supported_whgame_timestamp);
		message->set_whgame_image_size(supported_whgame_image_size);
		message->set_display_name(std::move(name));
		message->set_password("secret");
		return envelope;
	}

	protocol::Envelope enroll()
	{
		protocol::Envelope envelope;
		envelope.mutable_client_authenticate()->set_enroll(true);
		return envelope;
	}

	protocol::Envelope authenticate(std::string token)
	{
		protocol::Envelope envelope;
		envelope.mutable_client_authenticate()->set_identity_token(
		    std::move(token));
		return envelope;
	}

	protocol::Envelope claim(std::string code)
	{
		protocol::Envelope envelope;
		envelope.mutable_client_authenticate()->set_claim_code(
		    std::move(code));
		return envelope;
	}

	protocol::Envelope ready(const protocol::ServerBootstrap &bootstrap)
	{
		protocol::Envelope envelope;
		auto *message = envelope.mutable_client_world_ready();
		message->set_session_id(bootstrap.session_id());
		message->set_manifest_revision(bootstrap.manifest_revision());
		message->set_level_id(bootstrap.level_id());
		return envelope;
	}

	protocol::Envelope client_transform(
	    std::uint64_t sequence,
	    float x = 10.0F)
	{
		protocol::Envelope envelope;
		auto *transform =
		    envelope.mutable_client_transform()->mutable_transform();
		transform->mutable_position()->set_x(x);
		transform->mutable_position()->set_y(20.0F);
		transform->mutable_position()->set_z(30.0F);
		transform->mutable_rotation()->set_w(1.0F);
		transform->mutable_velocity();
		transform->set_sequence(sequence);
		transform->set_client_time_ms(sequence * 10);
		return envelope;
	}

	const protocol::ServerBootstrap &find_bootstrap(
	    const std::vector<outbound_message> &messages,
	    connection_id connection)
	{
		for (const auto &message : messages)
		{
			if (message.connection == connection
			    && message.envelope.has_server_bootstrap())
			{
				return message.envelope.server_bootstrap();
			}
		}
		std::cerr << "missing bootstrap for connection " << connection
		          << "; outbound payloads:";
		for (const auto &message : messages)
		{
			std::cerr << ' ' << message.connection << ':'
			          << static_cast<int>(message.envelope.payload_case());
			if (message.envelope.has_server_rejected())
			{
				std::cerr << '('
				          << message.envelope.server_rejected().message()
				          << ')';
			}
		}
		std::cerr << '\n';
		assert(false);
		std::abort();
	}

	bool has_accepted(
	    const std::vector<outbound_message> &messages,
	    connection_id connection,
	    player_id expected)
	{
		for (const auto &message : messages)
		{
			if (message.connection == connection
			    && message.envelope.has_server_accepted()
			    && message.envelope.server_accepted().player_id() == expected)
			{
				return true;
			}
		}
		return false;
	}

	bool has_rejection(
	    const std::vector<outbound_message> &messages,
	    connection_id connection,
	    protocol::RejectReason reason)
	{
		return std::ranges::any_of(
		    messages,
		    [&](const outbound_message &message)
		    {
			    return message.connection == connection
			        && message.envelope.has_server_rejected()
			        && message.envelope.server_rejected().reason() == reason;
		    });
	}

	std::string connect_new_player(
	    server_core &core,
	    connection_id connection,
	    time_point now,
	    player_id expected_id)
	{
		core.on_transport_connected(connection, now);
		core.on_message(connection, hello(), now);
		auto outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_server_challenge());

		core.on_message(connection, enroll(), now + 1ms);
		outbound = core.take_outbound();
		const auto bootstrap = find_bootstrap(outbound, connection);
		assert(bootstrap.mode() == protocol::BOOTSTRAP_MODE_LOAD);
		assert(!bootstrap.issued_identity_token().empty());
		const auto token = bootstrap.issued_identity_token();

		core.on_message(connection, ready(bootstrap), now + 2ms);
		outbound = core.take_outbound();
		assert(has_accepted(outbound, connection, expected_id));
		return token;
	}
}

int main()
{
	std::set_terminate(
	    []
	    {
		    if (const auto exception = std::current_exception())
		    {
			    try
			    {
				    std::rethrow_exception(exception);
			    }
			    catch (const std::exception &error)
			    {
				    std::cerr << "unhandled exception: " << error.what()
				              << '\n';
			    }
		    }
		    std::_Exit(1);
	    });
	using namespace kcd2mp;
	using namespace kcd2mp::server;
	const auto start = clock::now();

	temporary_world invalid_config_world;
	{
		auto invalid = config_for(invalid_config_world.path / "must-not-exist");
		invalid.max_players = 0;
		bool rejected = false;
		try
		{
			server_core core(invalid);
			(void)core;
		}
		catch (const std::exception &)
		{
			rejected = true;
		}
		assert(rejected);
		assert(!std::filesystem::exists(invalid.world_directory));
	}

	temporary_world persistent_world;
	std::string identity_token;
	player_id persistent_id{};
	{
		auto counter = 0;
		server_core core(
		    config_for(persistent_world.path),
		    [&]
		    {
			    return "test-token-" + std::to_string(++counter);
		    });
		identity_token = connect_new_player(core, 1, start, 1);
		persistent_id = core.players().front().id;
		core.on_message(1, client_transform(100), start + 3ms);
		assert(core.players().front().last_sequence == 100);

		protocol::Envelope update;
		auto *profile_update = update.mutable_client_profile_update();
		profile_update->set_base_revision(1);
		auto *profile = profile_update->mutable_profile();
		profile->set_player_id(persistent_id);
		profile->set_revision(1);
		profile->set_display_name("Henry");
		profile->set_level_id("sandbox");
		profile->set_money(1234);
		auto *stat = profile->add_stats();
		stat->set_id("strength");
		stat->set_level(7);
		stat->set_xp(42.0F);
		core.on_message(1, update, start + 4ms);
		auto outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.profile_accepted().revision() == 2);

		core.on_transport_connected(2, start + 5ms);
		core.on_message(2, hello(), start + 5ms);
		(void)core.take_outbound();
		core.on_message(2, authenticate(identity_token), start + 6ms);
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.server_rejected().reason()
		    == protocol::REJECT_REASON_IDENTITY_IN_USE);

		const auto claim_code =
		    core.create_profile_claim(persistent_id, start + 7ms);
		assert(claim_code);
		core.on_transport_disconnected(
		    1,
		    false,
		    "intentional disconnect",
		    start + 8ms);
		(void)core.take_outbound();

		core.on_transport_connected(3, start + 9ms);
		core.on_message(3, hello(), start + 9ms);
		(void)core.take_outbound();
		core.on_message(3, claim(*claim_code), start + 10ms);
		outbound = core.take_outbound();
		const auto reclaimed = find_bootstrap(outbound, 3);
		assert(!reclaimed.issued_identity_token().empty());
		assert(reclaimed.profile().money() == 1234);
		identity_token = reclaimed.issued_identity_token();
	}

	{
		server_core restarted(config_for(persistent_world.path));
		restarted.on_transport_connected(10, start + 1s);
		restarted.on_message(10, hello(), start + 1s);
		(void)restarted.take_outbound();
		restarted.on_message(
		    10,
		    authenticate(identity_token),
		    start + 1001ms);
		auto outbound = restarted.take_outbound();
		const auto bootstrap = find_bootstrap(outbound, 10);
		assert(bootstrap.profile().player_id() == persistent_id);
		assert(bootstrap.profile().money() == 1234);
		assert(bootstrap.profile().stats(0).id() == "strength");
		restarted.on_message(10, ready(bootstrap), start + 1002ms);
		(void)restarted.take_outbound();
		restarted.on_message(
		    10,
		    client_transform(1),
		    start + 1003ms);
		assert(restarted.players().front().last_sequence == 1);
	}

	temporary_world initializer_world;
	{
		auto counter = 0;
		server_core core(
		    config_for(initializer_world.path, false),
		    [&]
		    {
			    return "initializer-token-" + std::to_string(++counter);
		    });
		core.on_transport_connected(20, start);
		core.on_message(20, hello("Henry"), start);
		(void)core.take_outbound();
		core.on_message(20, enroll(), start + 1ms);
		auto outbound = core.take_outbound();
		const auto initializer = find_bootstrap(outbound, 20);
		assert(initializer.mode() == protocol::BOOTSTRAP_MODE_INITIALIZE);

		core.on_transport_connected(21, start + 2ms);
		core.on_message(21, hello("Hans"), start + 2ms);
		(void)core.take_outbound();
		core.on_message(21, enroll(), start + 3ms);
		outbound = core.take_outbound();
		assert(find_bootstrap(outbound, 21).mode()
		    == protocol::BOOTSTRAP_MODE_WAIT);

		auto initializer_ready = ready(initializer);
		auto *spawn =
		    initializer_ready.mutable_client_world_ready()
		        ->mutable_initial_spawn();
		spawn->mutable_position()->set_x(100.0F);
		spawn->mutable_position()->set_y(200.0F);
		spawn->mutable_position()->set_z(300.0F);
		spawn->mutable_rotation()->set_w(1.0F);
		spawn->mutable_velocity();
		initializer_ready.mutable_client_world_ready()
		    ->set_initialized_session(true);
		core.on_message(20, initializer_ready, start + 4ms);
		outbound = core.take_outbound();
		assert(has_accepted(outbound, 20, 1));
		assert(find_bootstrap(outbound, 21).mode()
		    == protocol::BOOTSTRAP_MODE_LOAD);
	}

	temporary_world capacity_world;
	{
		auto config = config_for(capacity_world.path);
		config.max_players = 1;
		server_core core(config);
		(void)connect_new_player(core, 30, start, 1);
		core.on_transport_connected(31, start + 1s);
		core.on_message(31, hello("Hans"), start + 1s);
		(void)core.take_outbound();
		core.on_message(31, enroll(), start + 1001ms);
		const auto outbound = core.take_outbound();
		assert(has_rejection(
		    outbound,
		    31,
		    protocol::REJECT_REASON_SERVER_FULL));
	}

	temporary_world lease_world;
	{
		auto config = config_for(lease_world.path, false);
		config.bootstrap_timeout_seconds = 30;
		server_core core(config);

		core.on_transport_connected(40, start);
		core.on_message(40, hello("Henry"), start);
		(void)core.take_outbound();
		core.on_message(40, enroll(), start + 1ms);
		assert(find_bootstrap(core.take_outbound(), 40).mode()
		    == protocol::BOOTSTRAP_MODE_INITIALIZE);

		core.on_transport_connected(41, start + 2ms);
		core.on_message(41, hello("Hans"), start + 2ms);
		(void)core.take_outbound();
		core.on_message(41, enroll(), start + 3ms);
		assert(find_bootstrap(core.take_outbound(), 41).mode()
		    == protocol::BOOTSTRAP_MODE_WAIT);

		core.on_transport_disconnected(
		    40,
		    false,
		    "initializer disconnected",
		    start + 4ms);
		assert(find_bootstrap(core.take_outbound(), 41).mode()
		    == protocol::BOOTSTRAP_MODE_INITIALIZE);

		core.tick(start + 31s);
		const auto outbound = core.take_outbound();
		assert(has_rejection(
		    outbound,
		    41,
		    protocol::REJECT_REASON_BOOTSTRAP_FAILED));
	}

	return 0;
}
