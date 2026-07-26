#include "multiplayer/networking.hpp"

#include <Windows.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <optional>
#include <string>
#include <thread>

int main()
{
	using namespace std::chrono_literals;
	using namespace kcd2mp;

	net::runtime runtime;
	std::optional<connection_id> server_connection;
	bool client_connected{};
	bool server_received{};
	bool client_received{};

	net::server_transport *server_ptr{};
	net::server_transport server({
	    .connected =
	        [&](connection_id connection)
	        {
		        server_connection = connection;
	        },
	    .disconnected =
	        [&](connection_id, bool, std::string)
	        {
		        server_connection.reset();
	        },
	    .message =
	        [&](connection_id connection, std::span<const std::byte> bytes)
	        {
		        const std::array expected{
		            std::byte{0x4B},
		            std::byte{0x43},
		            std::byte{0x44}};
		        server_received =
		            bytes.size() == expected.size()
		            && std::equal(bytes.begin(), bytes.end(), expected.begin());
		        std::string error;
		        assert(server_ptr->send(
		            connection,
		            expected,
		            reliability::unreliable,
		            &error));
	        }});
	server_ptr = &server;

	net::client_transport client({
	    .connected =
	        [&]
	        {
		        client_connected = true;
	        },
	    .disconnected =
	        [&](bool, std::string)
	        {
		        client_connected = false;
	        },
	    .message =
	        [&](std::span<const std::byte> bytes)
	        {
		        const std::array expected{
		            std::byte{0x4B},
		            std::byte{0x43},
		            std::byte{0x44}};
		        client_received =
		            bytes.size() == expected.size()
		            && std::equal(bytes.begin(), bytes.end(), expected.begin());
	        }});

	const auto port =
	    static_cast<std::uint16_t>(40000 + GetCurrentProcessId() % 10000);
	server.listen("127.0.0.1", port);
	client.connect("127.0.0.1:" + std::to_string(port));
	assert(!client.has_connection());
	{
		const std::array premature_message{std::byte{0x01}};
		std::string error;
		assert(!client.send(
		    premature_message,
		    reliability::reliable,
		    &error));
		assert(error == "client is not connected");
	}

	const auto deadline = std::chrono::steady_clock::now() + 5s;
	bool sent{};
	while (std::chrono::steady_clock::now() < deadline && !client_received)
	{
		server.poll();
		client.poll();
		if (client_connected && server_connection && !sent)
		{
			const std::array message{
			    std::byte{0x4B},
			    std::byte{0x43},
			    std::byte{0x44}};
			std::string error;
			assert(client.send(message, reliability::reliable, &error));
			sent = true;
		}
		std::this_thread::sleep_for(1ms);
	}

	assert(client_connected);
	assert(server_connection.has_value());
	assert(server_received);
	assert(client_received);
	assert(client.ping_ms() >= -1);
	assert(client.packet_loss_percent() >= 0.0F);
	assert(client.packet_loss_percent() <= 100.0F);
	client.disconnect("networking test complete");
	return 0;
}
