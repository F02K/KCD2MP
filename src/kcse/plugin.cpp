#include "kcse/client_api.hpp"
#include "kcse/native_runtime.hpp"
#include "multiplayer/client.hpp"
#include "multiplayer/protocol.hpp"

#include <KCSE/KCSEAPI.h>

#include <algorithm>
#include <cstring>
#include <limits>
#include <string>
#include <string_view>

namespace
{
	kcd2mp::kcse::native_runtime *g_runtime{};
	kcd2mp::multiplayer_client *g_client{};
	KCSE::ITaskInterface *g_tasks{};

	template<std::size_t N>
	void copy_text(char (&target)[N], std::string_view value) noexcept
	{
		const auto count = std::min(value.size(), N - 1);
		std::memcpy(target, value.data(), count);
		target[count] = '\0';
	}

	template<std::size_t N>
	bool valid_text(const char (&value)[N]) noexcept
	{
		return std::memchr(value, '\0', N) != nullptr;
	}

	std::uint32_t narrow_count(std::size_t count) noexcept
	{
		return static_cast<std::uint32_t>(std::min<std::size_t>(
		    count,
		    std::numeric_limits<std::uint32_t>::max()));
	}

	void queue_frame();

	void run_frame()
	{
		if (!g_runtime || !g_client || !g_tasks)
			return;
		if (g_runtime->on_frame())
			g_client->runtime_epoch_changed();

		auto client_status = g_client->status();
		if (client_status.state != kcd2mp::client_state::disconnected)
		{
			g_client->game_tick(
			    g_runtime->local_transform(),
			    g_runtime->local_avatar_visual(),
			    g_runtime->current_level_id());
			client_status = g_client->status();
		}

		if (client_status.state != kcd2mp::client_state::disconnected
		    && g_runtime->sandbox_active())
		{
			const auto players = g_client->remote_players();
			std::vector<kcd2mp::remote_avatar_snapshot> snapshots;
			snapshots.reserve(players.size());
			for (const auto &player : players)
			{
				snapshots.push_back({
				    .id = player.id,
				    .display_name = player.display_name,
				    .connected = player.connected,
				    .has_transform = player.has_transform,
				    .transform = player.transform,
				    .movement_mode = player.movement_mode,
				    .has_avatar = player.has_avatar,
				    .avatar = player.avatar});
			}
			const auto synchronized =
			    g_runtime->sync_remote_players(snapshots);
			if (!synchronized.success)
				g_client->fail(
				    "Native remote-avatar synchronization failed: "
				    + synchronized.error);
		}

		if (const auto correction = g_client->take_local_correction())
		{
			if (!g_runtime->apply_local_correction(*correction))
			{
				g_client->fail(
				    "Server correction requires a runtime-verified native "
				    "transform mutation wrapper.");
			}
		}
		if (client_status.state == kcd2mp::client_state::disconnected)
		{
			if (g_runtime->sandbox_active())
				g_runtime->end_sandbox();
			else
				g_runtime->cancel_multiplayer_preparation();
		}
		queue_frame();
	}

	void queue_frame()
	{
		if (!g_tasks)
			return;
		g_tasks->AddTask(run_frame);
	}

	void on_kcse_message(KCSE::Message *message)
	{
		if (message && g_runtime)
			g_runtime->on_lifecycle(message->type);
	}

	std::uint32_t __cdecl abi_get_runtime_status(
	    kcd2mp::kcse::runtime_status *result) noexcept
	{
		try
		{
			if (!result
			    || result->struct_size != sizeof(kcd2mp::kcse::runtime_status)
			    || !g_runtime)
				return 0;
			const auto descriptor = g_runtime->descriptor();
			const auto gate = g_runtime->capability();
			result->available = gate.available ? 1U : 0U;
			result->joinable = g_runtime->can_start_join() ? 1U : 0U;
			result->kcse_version = descriptor.kcse_version;
			result->game_version = descriptor.game_version;
			result->release_index = descriptor.release_index;
			result->epoch = descriptor.epoch;
			result->capabilities = descriptor.capabilities;
			copy_text(result->address_library, descriptor.address_library);
			copy_text(result->level_id, g_runtime->current_level_id());
			copy_text(result->diagnostic, gate.diagnostic);
			return 1;
		}
		catch (...)
		{
			return 0;
		}
	}

	std::uint32_t __cdecl abi_connect(
	    const kcd2mp::kcse::connect_request *request) noexcept
	{
		try
		{
			if (!request
			    || request->struct_size
			        != sizeof(kcd2mp::kcse::connect_request)
			    || !valid_text(request->address)
			    || !valid_text(request->display_name)
			    || !valid_text(request->password)
			    || !valid_text(request->content_hash)
			    || !valid_text(request->claim_code) || !g_client)
				return 0;
			kcd2mp::client_options options;
			options.address = request->address;
			options.display_name = request->display_name;
			options.password = request->password;
			options.content_hash = request->content_hash;
			options.claim_code = request->claim_code;
			return g_client->connect(std::move(options)) ? 1U : 0U;
		}
		catch (...)
		{
			return 0;
		}
	}

	void __cdecl abi_disconnect() noexcept
	{
		try
		{
			if (g_client)
				g_client->disconnect();
		}
		catch (...)
		{
		}
	}

	std::uint32_t __cdecl abi_send_chat(const char *text) noexcept
	{
		try
		{
			if (!text || !g_client)
				return 0;
			const auto length = strnlen_s(text, kcd2mp::kcse::text_capacity);
			if (length == 0 || length == kcd2mp::kcse::text_capacity)
				return 0;
			return g_client->send_chat(std::string(text, length)) ? 1U : 0U;
		}
		catch (...)
		{
			return 0;
		}
	}

	std::uint32_t __cdecl abi_select_avatar(
	    const char *archetype_id) noexcept
	{
		try
		{
			if (!archetype_id || !g_client)
				return 0;
			const auto length = strnlen_s(
			    archetype_id,
			    kcd2mp::kcse::short_text_capacity);
			if (length == 0
			    || length == kcd2mp::kcse::short_text_capacity)
				return 0;
			return g_client->select_avatar(
			           std::string(archetype_id, length))
			    ? 1U
			    : 0U;
		}
		catch (...)
		{
			return 0;
		}
	}

	std::uint32_t __cdecl abi_get_status(
	    kcd2mp::kcse::client_status_view *result) noexcept
	{
		try
		{
			if (!result
			    || result->struct_size
			        != sizeof(kcd2mp::kcse::client_status_view)
			    || !g_client)
				return 0;
			const auto status = g_client->status();
			result->state = static_cast<std::uint32_t>(status.state);
			result->local_player_id = status.local_player_id;
			result->ping_ms = status.ping_ms;
			result->packet_loss_percent = status.packet_loss_percent;
			result->game_queue_size = narrow_count(status.game_queue_size);
			copy_text(result->server_name, status.server_name);
			copy_text(result->server_id, status.server_id);
			copy_text(result->session_id, status.session_id);
			copy_text(result->level_id, status.level_id);
			copy_text(result->error, status.error);
			copy_text(
			    result->avatar_archetype_id,
			    status.avatar_archetype_id);
			copy_text(
			    result->default_avatar_archetype_id,
			    status.avatar_policy.default_archetype_id());
			return 1;
		}
		catch (...)
		{
			return 0;
		}
	}

	std::uint32_t __cdecl abi_copy_players(
	    kcd2mp::kcse::remote_player_view *output,
	    std::uint32_t capacity) noexcept
	{
		try
		{
			if (!g_client)
				return 0;
			const auto players = g_client->remote_players();
			if (!output || capacity == 0)
				return narrow_count(players.size());
			const auto count =
			    std::min<std::size_t>(players.size(), capacity);
			for (std::size_t index = 0; index < count; ++index)
			{
				output[index] = {};
				output[index].player_id = players[index].id;
				output[index].connected =
				    players[index].connected ? 1U : 0U;
				output[index].movement_mode =
				    static_cast<std::uint32_t>(
				        players[index].movement_mode);
				copy_text(
				    output[index].display_name,
				    players[index].display_name);
			}
			return narrow_count(count);
		}
		catch (...)
		{
			return 0;
		}
	}

	std::uint32_t __cdecl abi_copy_chat(
	    kcd2mp::kcse::chat_entry_view *output,
	    std::uint32_t capacity) noexcept
	{
		try
		{
			if (!g_client)
				return 0;
			const auto entries = g_client->chat_history();
			if (!output || capacity == 0)
				return narrow_count(entries.size());
			const auto count =
			    std::min<std::size_t>(entries.size(), capacity);
			for (std::size_t index = 0; index < count; ++index)
			{
				output[index] = {};
				output[index].player_id = entries[index].sender;
				output[index].server_time_ms =
				    entries[index].server_time_ms;
				copy_text(
				    output[index].display_name,
				    entries[index].display_name);
				copy_text(output[index].text, entries[index].text);
			}
			return narrow_count(count);
		}
		catch (...)
		{
			return 0;
		}
	}

	std::uint32_t __cdecl abi_copy_avatar_archetypes(
	    kcd2mp::kcse::fixed_string *output,
	    std::uint32_t capacity) noexcept
	{
		try
		{
			if (!g_client)
				return 0;
			const auto &policy = g_client->status().avatar_policy;
			const auto count = static_cast<std::size_t>(
			    policy.allowed_archetype_ids_size());
			if (!output || capacity == 0)
				return narrow_count(count);
			const auto written =
			    std::min<std::size_t>(count, capacity);
			for (std::size_t index = 0; index < written; ++index)
			{
				output[index] = {};
				copy_text(
				    output[index].value,
				    policy.allowed_archetype_ids(
				        static_cast<int>(index)));
			}
			return narrow_count(written);
		}
		catch (...)
		{
			return 0;
		}
	}

	const kcd2mp::kcse::client_api g_api{
	    sizeof(kcd2mp::kcse::client_api),
	    kcd2mp::kcse::client_abi_version,
	    kcd2mp::kcse::client_build_id,
	    abi_get_runtime_status,
	    abi_connect,
	    abi_disconnect,
	    abi_send_chat,
	    abi_select_avatar,
	    abi_get_status,
	    abi_copy_players,
	    abi_copy_chat,
	    abi_copy_avatar_archetypes};
}

KCSE_EXPORT KCSE::PluginVersionData KCSEPlugin_Version = {
    KCSE::PluginVersionData::kDataVersion,
    4,
    "KCD2MPClient",
    "F02K",
    {0x01050600},
    1,
    KCSE::PluginVersionData::kVersionIndependent_None,
};

KCSE_EXPORT bool KCSEPlugin_Load(const KCSE::IKCSEInterface *kcse)
{
	if (!kcse || kcse->GetKCSEVersion() < 1)
		return false;
	KCSE::Init(kcse);
	g_tasks = kcse->GetTaskInterface();
	auto *messaging = kcse->GetMessagingInterface();
	if (!g_tasks || !messaging)
		return false;

	// KCSE plugins are process-lifetime objects. Intentionally leak the client
	// so its network thread is never joined from DLL_PROCESS_DETACH.
	g_runtime = new kcd2mp::kcse::native_runtime(*kcse);
	g_client = new kcd2mp::multiplayer_client(*g_runtime);
	if (!messaging->RegisterListener("KCSE", on_kcse_message))
	{
		delete g_client;
		delete g_runtime;
		g_client = nullptr;
		g_runtime = nullptr;
		return false;
	}
	queue_frame();
	return true;
}

KCSE_EXPORT const kcd2mp::kcse::client_api *__cdecl KCD2MP_QueryClient(
    std::uint32_t requested_abi) noexcept
{
	return requested_abi == kcd2mp::kcse::client_abi_version ? &g_api :
	                                                           nullptr;
}
