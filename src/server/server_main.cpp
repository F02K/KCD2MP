#include "multiplayer/networking.hpp"
#include "multiplayer/protocol.hpp"
#include "server/server_config.hpp"
#include "server/server_core.hpp"

#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

namespace
{
	struct command_queue
	{
		std::mutex mutex;
		std::deque<std::string> commands;
	};

	void print_help()
	{
		std::cout
		    << "Commands: status, players, kick <player_id> [reason], "
		       "say <text>, profile claim <player_id>, "
		       "dummy spawn [name], dummy remove <player_id>, "
		       "entities <disable|enable|status>, stop, help\n";
	}

	int close_reason(kcd2mp::server::close_kind kind)
	{
		using kcd2mp::server::close_kind;
		switch (kind)
		{
		case close_kind::reject:
			return kcd2mp::net::server_rejected_reason;
		case close_kind::shutdown:
			return kcd2mp::net::server_shutdown_reason;
		case close_kind::kick:
			return kcd2mp::net::server_kicked_reason;
		case close_kind::none:
			break;
		}
		return 0;
	}
}

int main(int argc, char **argv)
{
	using namespace std::chrono_literals;
	try
	{
		const auto config_path = argc > 1
		    ? std::filesystem::path(argv[1])
		    : std::filesystem::path("server.toml");
		const auto config = kcd2mp::server::load_server_config(config_path);
		kcd2mp::net::runtime network_runtime;
		kcd2mp::server::server_core core(config);

		auto console = std::make_shared<command_queue>();
		std::thread(
		    [console]
		    {
			    std::string line;
			    while (std::getline(std::cin, line))
			    {
				    if (line.starts_with("\xEF\xBB\xBF"))
				    {
					    line.erase(0, 3);
				    }
				    std::scoped_lock lock(console->mutex);
				    console->commands.push_back(std::move(line));
			    }
		    })
		    .detach();

		const auto now = []
		{
			return kcd2mp::server::clock::now();
		};

		kcd2mp::net::server_transport transport({
		    .connected =
		        [&](kcd2mp::connection_id connection)
		        {
			        std::cout << "connection " << connection << " accepted from "
			                  << transport.connection_description(connection) << '\n';
			        core.on_transport_connected(connection, now());
		        },
		    .disconnected =
		        [&](kcd2mp::connection_id connection,
		            bool allow_reconnect,
		            std::string reason)
		        {
			        std::cout << "connection " << connection << " closed: "
			                  << reason << '\n';
			        core.on_transport_disconnected(
			            connection,
			            allow_reconnect,
			            std::move(reason),
			            now());
		        },
		    .message =
		        [&](kcd2mp::connection_id connection,
		            std::span<const std::byte> bytes)
		        {
			        std::string error;
			        const auto envelope = kcd2mp::decode(bytes, &error);
			        if (!envelope)
			        {
				        std::cerr << "connection " << connection
				                  << " sent malformed data: " << error << '\n';
				        transport.close(
				            connection,
				            kcd2mp::net::server_rejected_reason,
				            "malformed message",
				            false);
				        return;
			        }
			        core.on_message(connection, *envelope, now());
		        }});

		transport.listen(config.bind_address, config.port);
		std::cout << config.name << " listening on " << config.bind_address << ':'
		          << config.port << " for level " << config.level_id << '\n';
		print_help();

		bool running = true;
		const auto tick_duration =
		    std::chrono::duration<double>(1.0 / config.tick_rate);
		auto next_tick = kcd2mp::server::clock::now();
		while (running)
		{
			transport.poll();

			std::deque<std::string> commands;
			{
				std::scoped_lock lock(console->mutex);
				commands.swap(console->commands);
			}
			for (const auto &line : commands)
			{
				std::istringstream input(line);
				std::string command;
				input >> command;
				if (command == "status")
				{
					std::cout << "players=" << core.players().size()
					          << '/' << config.max_players
					          << " pending=" << core.pending_connection_count()
					          << " tick=" << core.server_tick()
					          << " non_player_entities="
					          << (core.non_player_entities_disabled()
					                  ? "disabled"
					                  : "enabled")
					          << '\n';
				}
				else if (command == "players")
				{
					for (const auto &player : core.players())
					{
						std::cout << player.id << "  " << player.display_name
						          << "  "
						          << (player.dummy
						                  ? "dummy"
						                  : (player.connected
						                         ? "connected"
						                         : "reconnecting"))
						          << '\n';
					}
				}
				else if (command == "kick")
				{
					kcd2mp::player_id id{};
					input >> id;
					std::string reason;
					std::getline(input >> std::ws, reason);
					core.kick(
					    id,
					    reason.empty() ? "kicked by server" : reason,
					    now());
				}
				else if (command == "say")
				{
					std::string text;
					std::getline(input >> std::ws, text);
					core.server_say(std::move(text), now());
				}
				else if (command == "dummy")
				{
					std::string action;
					input >> action;
					if (action == "spawn")
					{
						std::string name;
						std::getline(input >> std::ws, name);
						std::string error;
						if (const auto id =
						        core.spawn_dummy(std::move(name), &error))
						{
							std::cout << "spawned dummy player " << *id
							          << '\n';
						}
						else
						{
							std::cout << "could not spawn dummy: " << error
							          << '\n';
						}
					}
					else if (action == "remove")
					{
						kcd2mp::player_id id{};
						input >> id;
						if (id == 0 || !core.remove_dummy(id, now()))
						{
							std::cout << "unknown dummy player\n";
						}
						else
						{
							std::cout << "removed dummy player " << id
							          << '\n';
						}
					}
					else
					{
						std::cout
						    << "usage: dummy <spawn [name]|remove <player_id>>\n";
					}
				}
				else if (command == "profile")
				{
					std::string action;
					kcd2mp::player_id id{};
					input >> action >> id;
					if (action != "claim" || id == 0)
					{
						std::cout << "usage: profile claim <player_id>\n";
					}
					else if (const auto code =
					             core.create_profile_claim(id, now()))
					{
						std::cout << "claim code for " << id << ": " << *code
						          << " (valid for 10 minutes)\n";
					}
					else
					{
						std::cout << "unknown player profile\n";
					}
				}
				else if (command == "entities")
				{
					std::string action;
					input >> action;
					if (action == "disable" || action == "enable")
					{
						const bool disabled = action == "disable";
						const bool changed =
						    core.set_non_player_entities_disabled(disabled);
						std::cout << "non-player entities "
						          << (disabled ? "disabled" : "enabled")
						          << (changed ? "" : " (unchanged)") << '\n';
					}
					else if (action == "status")
					{
						std::cout << "non-player entities are "
						          << (core.non_player_entities_disabled()
						                  ? "disabled"
						                  : "enabled")
						          << '\n';
					}
					else
					{
						std::cout
						    << "usage: entities <disable|enable|status>\n";
					}
				}
				else if (command == "stop")
				{
					core.shutdown("server stopped");
					running = false;
				}
				else if (command == "help")
				{
					print_help();
				}
				else if (!command.empty())
				{
					std::cerr << "unknown command: " << command << '\n';
				}
			}

			const auto tick_now = now();
			if (tick_now >= next_tick)
			{
				core.tick(tick_now);
				next_tick = tick_now
				    + std::chrono::duration_cast<kcd2mp::server::clock::duration>(
				        tick_duration);
			}

			for (auto &outbound : core.take_outbound())
			{
				std::string error;
				const auto encoded =
				    kcd2mp::encode(outbound.envelope, outbound.delivery, &error);
				if (!encoded
				    || !transport.send(
				        outbound.connection,
				        encoded->bytes,
				        outbound.delivery,
				        &error))
				{
					std::cerr << "send to connection " << outbound.connection
					          << " failed: " << error << '\n';
				}
				if (outbound.close_after_send
				    != kcd2mp::server::close_kind::none)
				{
					transport.close(
					    outbound.connection,
					    close_reason(outbound.close_after_send),
					    "connection closed by server",
					    true);
				}
			}

			std::this_thread::sleep_for(1ms);
		}
		return 0;
	}
	catch (const std::exception &exception)
	{
		std::cerr << "KCD2MPServer fatal error: " << exception.what() << '\n';
		return 1;
	}
}
