#include "server/server_core.hpp"

#include <Windows.h>

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
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
		message->set_version(kcd2mp_version);
		message->set_whgame_timestamp(supported_whgame_timestamp);
		message->set_whgame_image_size(supported_whgame_image_size);
		message->set_display_name(std::move(name));
		message->set_password("secret");
		auto *runtime = message->mutable_runtime();
		runtime->set_features(required_client_runtime_capabilities);
		runtime->set_kcse_version(1);
		runtime->set_game_version(0x01050600);
		runtime->set_release_index(1);
		runtime->set_runtime_epoch(1);
		const auto &address_library = supported_address_libraries.back();
		runtime->set_address_library(address_library.build_key);
		runtime->set_address_library_distribution(
		    address_library.distribution);
		runtime->set_address_library_format(address_library.format_version);
		runtime->set_address_library_entries(address_library.entry_count);
		runtime->set_address_library_sha256(address_library.sha256);
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
		assert(bootstrap.profile().has_avatar());
		*message->mutable_avatar() = bootstrap.profile().avatar();
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

	bool has_entity_control(
	    const std::vector<outbound_message> &messages,
	    connection_id connection,
	    bool humans_disabled,
	    bool animals_disabled)
	{
		return std::ranges::any_of(
		    messages,
		    [&](const outbound_message &message)
			{
				if (message.connection != connection
				    || !message.envelope.has_server_entity_control())
					return false;
				const auto &control =
				    message.envelope.server_entity_control();
				return control.has_human_npcs_disabled()
				    && control.human_npcs_disabled() == humans_disabled
				    && control.has_animal_npcs_disabled()
				    && control.animal_npcs_disabled() == animals_disabled
				    && control.non_player_entities_disabled()
				        == (humans_disabled && animals_disabled);
			});
	}

	std::string connect_new_player(
	    server_core &core,
	    connection_id connection,
	    time_point now,
	    player_id expected_id,
	    protocol::PlayerProfile *enrolled_profile = nullptr,
	    std::string name = "Henry")
	{
		core.on_transport_connected(connection, now);
		core.on_message(connection, hello(std::move(name)), now);
		auto outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_server_challenge());

		core.on_message(connection, enroll(), now + 1ms);
		outbound = core.take_outbound();
		const auto bootstrap = find_bootstrap(outbound, connection);
		assert(bootstrap.mode() == protocol::BOOTSTRAP_MODE_LOAD);
		assert(bootstrap.has_environment());
		assert(is_valid_environment_state(bootstrap.environment()));
		assert(!bootstrap.issued_identity_token().empty());
		const auto token = bootstrap.issued_identity_token();
		if (enrolled_profile)
			*enrolled_profile = bootstrap.profile();

		core.on_message(connection, ready(bootstrap), now + 2ms);
		outbound = core.take_outbound();
		assert(has_accepted(outbound, connection, expected_id));
		assert(has_entity_control(
		    outbound,
		    connection,
		    core.human_npcs_disabled(),
		    core.animal_npcs_disabled()));
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

	temporary_world parsed_config_world;
	{
		const auto path = parsed_config_world.path / "server.toml";
		std::filesystem::copy_file(
		    std::filesystem::path(KCD2MP_SOURCE_DIR) / "starter_profile.toml",
		    parsed_config_world.path / "starter_profile.toml");
		std::ofstream output(path);
		output
		    << "[server]\n"
		       "level_id = \"sandbox\"\n"
		       "world_directory = \"world\"\n"
		       "disable_non_player_entities = true\n"
		       "[environment]\n"
		       "initial_time_of_day_hours = 21.5\n"
		       "time_scale = 30.0\n"
		       "weather_id = 8\n"
		       "weather_transition_seconds = 12\n";
		output.close();
		const auto parsed = load_server_config(path);
		assert(parsed.disable_human_npcs);
		assert(parsed.disable_animal_npcs);
		assert(parsed.world_directory
		    == parsed_config_world.path / "world");
		assert(parsed.initial_time_of_day_hours == 21.5);
		assert(parsed.time_scale == 30.0F);
		assert(parsed.weather_id == 8);
		assert(parsed.weather_transition_seconds == 12);
	}

	temporary_world environment_world;
	{
		auto config = config_for(environment_world.path);
		config.initial_time_of_day_hours = 23.5;
		config.time_scale = 600.0F;
		config.weather_id = 2;
		config.weather_transition_seconds = 5;
		config.idle_timeout_seconds = 300;
		server_core core(config);
		(void)connect_new_player(core, 93, start, 1);
		auto state = core.current_environment(start + 126s);
		assert(std::abs(state.time_of_day_hours() - 20.5) < 0.000001);
		assert(state.time_scale() == 600.0F);
		assert(state.weather_id() == 2);
		const auto revision = state.revision();
		assert(core.set_time_scale(15.0F, start + 126s));
		auto outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().delivery == reliability::reliable);
		assert(outbound.front().envelope.has_server_environment_updated());
		state = core.current_environment(start + 126s);
		assert(state.revision() == revision + 1);
		assert(std::abs(state.time_of_day_hours() - 20.5) < 0.000001);
		assert(core.set_time_of_day(6.25, start + 126s));
		(void)core.take_outbound();
		assert(core.set_weather(13, 30, start + 126s));
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front()
		           .envelope.server_environment_updated()
		           .state()
		           .weather_id()
		    == 13);
		state = core.current_environment(start + 126s);
		assert(std::abs(state.time_of_day_hours() - 6.25) < 0.000001);
		assert(state.weather_id() == 13);
		assert(state.weather_transition_ms() == 30'000);
		assert(!core.set_weather(34, 30, start + 126s));
		core.tick(start + 127s);
		outbound = core.take_outbound();
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.delivery == reliability::unreliable
			        && message.envelope.has_world_snapshot()
			        && message.envelope.world_snapshot().has_environment()
			        && message.envelope.world_snapshot().environment().weather_id()
			            == 13;
		    }));
	}
	{
		const auto path = parsed_config_world.path / "split-server.toml";
		std::ofstream output(path);
		output
		    << "[server]\n"
		       "level_id = \"sandbox\"\n"
		       "world_directory = \"world\"\n"
		       "starter_profile = \"starter_profile.toml\"\n"
		       "disable_non_player_entities = true\n"
		       "disable_animal_npcs = false\n";
		output.close();
		const auto parsed = load_server_config(path);
		assert(parsed.disable_human_npcs);
		assert(!parsed.disable_animal_npcs);
	}

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

	temporary_world incomplete_runtime_world;
	{
		server_core core(config_for(incomplete_runtime_world.path));
		core.on_transport_connected(89, start);
		auto wrong_version = hello();
		wrong_version.mutable_client_hello()->set_version("0.0.8");
		core.on_message(89, wrong_version, start);
		auto outbound = core.take_outbound();
		assert(has_rejection(
		    outbound,
		    89,
		    protocol::REJECT_REASON_VERSION_MISMATCH));

		core.on_transport_connected(90, start);
		auto missing_capability = hello();
		missing_capability.mutable_client_hello()
		    ->mutable_runtime()
		    ->set_features(runtime_capability_kcse);
		core.on_message(90, missing_capability, start);
		outbound = core.take_outbound();
		assert(has_rejection(
		    outbound,
		    90,
		    protocol::REJECT_REASON_GAME_BUILD_MISMATCH));

		core.on_transport_connected(92, start);
		auto wrong_address_library = hello();
		wrong_address_library.mutable_client_hello()
		    ->mutable_runtime()
		    ->set_address_library_sha256(std::string(64, '0'));
		core.on_message(92, wrong_address_library, start);
		outbound = core.take_outbound();
		assert(has_rejection(
		    outbound,
		    92,
		    protocol::REJECT_REASON_GAME_BUILD_MISMATCH));

		core.on_transport_connected(91, start);
		auto missing_address_library = hello();
		missing_address_library.mutable_client_hello()
		    ->mutable_runtime()
		    ->clear_address_library();
		core.on_message(91, missing_address_library, start);
		outbound = core.take_outbound();
		assert(has_rejection(
		    outbound,
		    91,
		    protocol::REJECT_REASON_GAME_BUILD_MISMATCH));
	}

	temporary_world persistent_world;
	std::string identity_token;
	player_id persistent_id{};
	{
		auto counter = 0;
		protocol::PlayerProfile enrolled_profile;
		server_core core(
		    config_for(persistent_world.path),
		    [&]
		    {
			    return "test-token-" + std::to_string(++counter);
		    });
		identity_token = connect_new_player(
		    core,
		    1,
		    start,
		    1,
		    &enrolled_profile);
		persistent_id = core.players().front().id;
		core.on_message(1, client_transform(100), start + 3ms);
		assert(core.players().front().last_sequence == 100);

		protocol::Envelope update;
		auto *profile_update = update.mutable_client_profile_update();
		profile_update->set_base_revision(enrolled_profile.revision());
		auto *profile = profile_update->mutable_profile();
		*profile = enrolled_profile;
		profile->set_money(profile->money() + 1);
		profile->set_money_subunits(7);
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
		assert(reclaimed.profile().revision() == 2);
		assert(reclaimed.profile().money_subunits() == 7);
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
		assert(bootstrap.profile().revision() == 2);
		assert(bootstrap.profile().money_subunits() == 7);
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

	temporary_world entity_control_world;
	{
		auto config = config_for(entity_control_world.path);
		config.disable_human_npcs = true;
		config.disable_animal_npcs = false;
		server_core core(config);
		assert(core.human_npcs_disabled());
		assert(!core.animal_npcs_disabled());
		(void)connect_new_player(core, 35, start, 1);

		assert(!core.set_npc_entities_disabled(true, false));
		assert(core.take_outbound().empty());
		assert(core.set_npc_entities_disabled(false, true));
		auto outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(has_entity_control(outbound, 35, false, true));
		assert(!core.human_npcs_disabled());
		assert(core.animal_npcs_disabled());
		assert(!core.set_npc_entities_disabled(false, true));
		assert(core.take_outbound().empty());
	}

	temporary_world dummy_world;
	{
		server_core core(config_for(dummy_world.path));
		(void)connect_new_player(core, 36, start, 1);

		std::string error;
		const auto dummy_id =
		    core.spawn_dummy("Training Dummy", &error);
		assert(dummy_id);
		assert(error.empty());
		const auto players_with_dummy = core.players();
		assert(players_with_dummy.size() == 2);
		const auto dummy = std::ranges::find(
		    players_with_dummy,
		    *dummy_id,
		    &player_view::id);
		assert(dummy != players_with_dummy.end());
		assert(dummy->dummy);
		assert(dummy->connected);
		assert(dummy->has_transform);
		auto outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().connection == 36);
		assert(outbound.front().envelope.has_player_joined());
		const auto &joined =
		    outbound.front().envelope.player_joined().player();
		assert(joined.player_id() == *dummy_id);
		assert(joined.display_name() == "Training Dummy");
		assert(joined.connected());
		assert(joined.transform_valid());
		assert(joined.transform().position().x() == 12.0F);
		assert(joined.has_avatar());
		assert(
		    joined.avatar().archetype_id()
		    == core.config().default_avatar_archetype);
		assert(joined.avatar().revision() == 1);
		assert(encode(
		    outbound.front().envelope,
		    outbound.front().delivery));
		assert(!core.create_profile_claim(*dummy_id, start + 1ms));

		assert(!core.spawn_dummy("Training Dummy", &error));
		assert(error == "display name is already in use");
		assert(core.take_outbound().empty());

		assert(core.remove_dummy(*dummy_id, start + 2ms));
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_player_left());
		assert(
		    outbound.front().envelope.player_left().player_id()
		    == *dummy_id);
		assert(core.players().size() == 1);
		assert(!core.remove_dummy(*dummy_id, start + 3ms));
	}

	temporary_world avatar_world;
	{
		constexpr std::string_view knight_soul =
		    "11111111-2222-4333-8444-555555555555";
		auto config = config_for(avatar_world.path);
		config.known_avatar_archetypes.insert(std::string(knight_soul));
		config.allowed_avatar_archetypes.push_back(std::string(knight_soul));
		server_core core(config);
		protocol::PlayerProfile enrolled_profile;
		(void)connect_new_player(
		    core,
		    37,
		    start,
		    1,
		    &enrolled_profile);

		protocol::Envelope inventory_update;
		auto *inventory_message =
		    inventory_update.mutable_client_profile_update();
		inventory_message->set_base_revision(enrolled_profile.revision());
		*inventory_message->mutable_profile() = enrolled_profile;
		auto *inventory_item =
		    inventory_message->mutable_profile()->add_inventory();
		inventory_item->set_instance_id(
		    "bbbbbbbb-bbbb-4bbb-8bbb-bbbbbbbbbbbb");
		inventory_item->set_definition_id(
		    "aaaaaaaa-aaaa-4aaa-8aaa-aaaaaaaaaaaa");
		inventory_item->set_count(1);
		inventory_item->set_quality(1.0F);
		inventory_item->set_condition(1.0F);
		inventory_item->set_equipped_slot("PrimaryMainHand");
		core.on_message(37, inventory_update, start + 3ms);
		auto outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_profile_accepted());

		protocol::Envelope update;
		auto *message = update.mutable_client_avatar_update();
		message->set_base_revision(1);
		auto *avatar = message->mutable_avatar();
		avatar->set_archetype_id(std::string(knight_soul));
		avatar->set_revision(1);
		avatar->set_stance(protocol::AVATAR_STANCE_READY);
		avatar->set_weapon_class(
		    protocol::AVATAR_WEAPON_CLASS_ONE_HANDED);
		avatar->set_weapon_drawn(true);
		auto *item = avatar->add_equipment();
		item->set_definition_id(
		    "aaaaaaaa-aaaa-4aaa-8aaa-aaaaaaaaaaaa");
		item->set_equipped_slot("PrimaryMainHand");
		core.on_message(37, update, start + 4ms);
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_avatar_accepted());
		assert(outbound.front().envelope.avatar_accepted().revision() == 2);

		core.on_message(37, update, start + 5ms);
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_avatar_rejected());
		assert(outbound.front()
		           .envelope.avatar_rejected()
		           .authoritative_avatar()
		           .revision()
		    == 2);
		assert(outbound.front()
		           .envelope.avatar_rejected()
		           .authoritative_avatar()
		           .archetype_id()
		    == knight_soul);

		message->set_base_revision(2);
		avatar->set_revision(2);
		avatar->set_archetype_id(
		    "99999999-9999-4999-8999-999999999999");
		core.on_message(37, update, start + 6ms);
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_avatar_rejected());
		assert(outbound.front()
		           .envelope.avatar_rejected()
		           .authoritative_avatar()
		           .revision()
		    == 3);
		assert(outbound.front()
		           .envelope.avatar_rejected()
		           .authoritative_avatar()
		           .archetype_id()
		    == npc::default_soul_id);

		message->set_base_revision(3);
		avatar->set_revision(3);
		avatar->mutable_equipment(0)->set_definition_id(
		    "not-a-runtime-item-id");
		core.on_message(37, update, start + 2s);
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_avatar_rejected());
		assert(outbound.front().close_after_send == close_kind::none);
		assert(outbound.front()
		           .envelope.avatar_rejected()
		           .authoritative_avatar()
		           .revision()
		    == 3);
		assert(core.players().size() == 1);
	}

	{
		server_config config;
		config.default_avatar_archetype = "unknown.default";
		config.allowed_avatar_archetypes = {
		    "unknown.allowed",
		    std::string(npc::default_soul_id),
		    "unknown.allowed"};
		normalize_avatar_config(config);
		assert(config.default_avatar_archetype == npc::default_soul_id);
		assert(config.allowed_avatar_archetypes.size() == 1);
		assert(
		    config.allowed_avatar_archetypes.front()
		    == npc::default_soul_id);

		const std::string custom =
		    "11111111-2222-4333-8444-555555555555";
		config.known_avatar_archetypes.insert(custom);
		config.default_avatar_archetype = custom;
		config.allowed_avatar_archetypes = {custom};
		normalize_avatar_config(config);
		assert(config.allowed_avatar_archetypes.size() == 2);
		assert(std::ranges::find(
		           config.allowed_avatar_archetypes,
		           npc::default_soul_id)
		    != config.allowed_avatar_archetypes.end());
	}

	temporary_world world_sync_world;
	std::string world_identity_token;
	{
		server_core core(config_for(world_sync_world.path));
		protocol::PlayerProfile first_profile;
		protocol::PlayerProfile second_profile;
		world_identity_token = connect_new_player(
		    core,
		    50,
		    start,
		    1,
		    &first_profile);
		(void)connect_new_player(
		    core,
		    51,
		    start + 4ms,
		    2,
		    &second_profile,
		    "Hans");

		protocol::Envelope observed_container;
		auto *container_update =
		    observed_container.mutable_client_world_object_update();
		container_update->set_base_revision(0);
		auto *container = container_update->mutable_state();
		container->set_entity_guid(0x12345678ULL);
		container->set_kind(protocol::WORLD_OBJECT_KIND_CONTAINER);
		container->set_revision(0);
		container->set_opened(true);
		container->set_has_inventory(true);
		auto *loot = container->add_inventory();
		loot->set_instance_id(
		    "cccccccc-cccc-4ccc-8ccc-cccccccccccc");
		loot->set_definition_id(
		    "dddddddd-dddd-4ddd-8ddd-dddddddddddd");
		loot->set_count(1);
		loot->set_quality(100.0F);
		loot->set_condition(1.0F);
		core.on_message(50, observed_container, start + 8ms);
		auto outbound = core.take_outbound();
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.connection == 50
			        && message.envelope.has_world_object_accepted()
			        && message.envelope.world_object_accepted().revision() == 1;
		    }));
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.connection == 51
			        && message.envelope.has_world_object_updated()
			        && message.envelope.world_object_updated()
			               .state()
			               .inventory_size()
			            == 1;
		    }));

		protocol::Envelope first_loot_update;
		auto *first_profile_update =
		    first_loot_update.mutable_client_profile_update();
		first_profile_update->set_base_revision(first_profile.revision());
		*first_profile_update->mutable_profile() = first_profile;
		*first_profile_update->mutable_profile()->add_inventory() = *loot;
		core.on_message(50, first_loot_update, start + 9ms);
		outbound = core.take_outbound();
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.connection == 50
			        && message.envelope.has_profile_accepted()
			        && message.envelope.profile_accepted().revision() == 2;
		    }));
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.envelope.has_world_object_updated()
			        && message.envelope.world_object_updated().state().revision()
			            == 2
			        && message.envelope.world_object_updated()
			               .state()
			               .inventory_size()
			            == 0;
		    }));

		protocol::Envelope duplicate_loot_update;
		auto *second_profile_update =
		    duplicate_loot_update.mutable_client_profile_update();
		second_profile_update->set_base_revision(second_profile.revision());
		*second_profile_update->mutable_profile() = second_profile;
		*second_profile_update->mutable_profile()->add_inventory() = *loot;
		core.on_message(51, duplicate_loot_update, start + 10ms);
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_profile_rejected());
		assert(outbound.front().close_after_send == close_kind::none);
		assert(outbound.front()
		           .envelope.profile_rejected()
		           .authoritative_profile()
		           .revision()
		    == second_profile.revision());

		auto stale_container = observed_container;
		stale_container.mutable_client_world_object_update()
		    ->set_base_revision(1);
		stale_container.mutable_client_world_object_update()
		    ->mutable_state()
		    ->set_revision(1);
		stale_container.mutable_client_world_object_update()
		    ->mutable_state()
		    ->set_opened(false);
		core.on_message(51, stale_container, start + 11ms);
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_world_object_rejected());
		const auto &authoritative = outbound.front()
		                                .envelope.world_object_rejected()
		                                .authoritative_state();
		assert(authoritative.revision() == 2);
		assert(authoritative.inventory_size() == 0);

		auto reintroduced_loot = observed_container;
		reintroduced_loot.mutable_client_world_object_update()
		    ->set_base_revision(2);
		reintroduced_loot.mutable_client_world_object_update()
		    ->mutable_state()
		    ->set_revision(2);
		core.on_message(51, reintroduced_loot, start + 12ms);
		outbound = core.take_outbound();
		assert(outbound.size() == 1);
		assert(outbound.front().envelope.has_world_object_rejected());
		assert(outbound.front()
		           .envelope.world_object_rejected()
		           .authoritative_state()
		           .revision()
		    == 2);
		assert(outbound.front()
		           .envelope.world_object_rejected()
		           .authoritative_state()
		           .inventory_size()
		    == 0);

		protocol::Envelope dropped_item;
		auto *drop_update = dropped_item.mutable_client_world_item_update();
		drop_update->set_base_revision(0);
		auto *drop = drop_update->mutable_state();
		drop->set_instance_id(loot->instance_id());
		drop->set_revision(0);
		drop->set_present(true);
		*drop->mutable_item() = *loot;
		auto *drop_transform = drop->mutable_transform();
		drop_transform->mutable_position()->set_x(1.0F);
		drop_transform->mutable_position()->set_y(2.0F);
		drop_transform->mutable_position()->set_z(3.0F);
		drop_transform->mutable_rotation()->set_w(1.0F);
		drop_transform->mutable_velocity();
		core.on_message(50, dropped_item, start + 13ms);
		outbound = core.take_outbound();
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.connection == 50
			        && message.envelope.has_world_item_accepted()
			        && message.envelope.world_item_accepted().revision() == 1;
		    }));
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.connection == 51
			        && message.envelope.has_world_item_updated()
			        && message.envelope.world_item_updated().state().present();
		    }));

		protocol::Envelope picked_up_item;
		auto *pickup_profile =
		    picked_up_item.mutable_client_profile_update();
		pickup_profile->set_base_revision(second_profile.revision());
		*pickup_profile->mutable_profile() = second_profile;
		*pickup_profile->mutable_profile()->add_inventory() = *loot;
		core.on_message(51, picked_up_item, start + 14ms);
		outbound = core.take_outbound();
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.connection == 51
			        && message.envelope.has_profile_accepted();
		    }));
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.envelope.has_world_item_updated()
			        && !message.envelope.world_item_updated().state().present()
			        && message.envelope.world_item_updated().state().revision() == 2;
		    }));
	}
	{
		server_core restarted(config_for(world_sync_world.path));
		restarted.on_transport_connected(52, start + 1s);
		restarted.on_message(52, hello(), start + 1s);
		(void)restarted.take_outbound();
		restarted.on_message(
		    52,
		    authenticate(world_identity_token),
		    start + 1s + 1ms);
		const auto bootstrap = find_bootstrap(restarted.take_outbound(), 52);
		assert(bootstrap.world_objects_size() == 0);
		restarted.on_message(52, ready(bootstrap), start + 1s + 2ms);
		const auto outbound = restarted.take_outbound();
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.connection == 52
			        && message.envelope.has_world_object_updated()
			        && message.envelope.world_object_updated()
			               .state()
			               .entity_guid()
			            == 0x12345678ULL
			        && message.envelope.world_object_updated().state().revision()
			            == 2
			        && message.envelope.world_object_updated()
			               .state()
			               .inventory_size()
			            == 0;
		    }));
		assert(std::ranges::any_of(
		    outbound,
		    [](const outbound_message &message)
		    {
			    return message.connection == 52
			        && message.envelope.has_world_item_updated()
			        && !message.envelope.world_item_updated().state().present()
			        && message.envelope.world_item_updated().state().revision() == 2;
		    }));
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
