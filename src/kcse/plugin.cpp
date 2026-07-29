#include "kcse/bridge_api.hpp"

#include "KCSE/KCSEAPI.h"
#include "crysystem/SSystemGlobalEnvironment.h"
#include "game/S_GameContext.h"

#include <atomic>

namespace
{
	std::atomic<std::uint32_t> g_runtime_flags{};
	const KCSE::IKCSEInterface *g_kcse{};

	void on_kcse_message(KCSE::Message *message)
	{
		if (!message)
		{
			return;
		}
		switch (message->type)
		{
		case KCSE::IMessagingInterface::kMessage_AllPluginsLoaded:
			g_runtime_flags.fetch_or(
			    kcd2mp::kcse::runtime_all_plugins_loaded,
			    std::memory_order_release);
			break;
		case KCSE::IMessagingInterface::kMessage_DataLoaded:
			g_runtime_flags.fetch_or(
			    kcd2mp::kcse::runtime_data_loaded,
			    std::memory_order_release);
			break;
		case KCSE::IMessagingInterface::kMessage_LoadGame:
		case KCSE::IMessagingInterface::kMessage_NewGame:
			g_runtime_flags.fetch_or(
			    kcd2mp::kcse::runtime_save_loaded,
			    std::memory_order_release);
			break;
		default:
			break;
		}
	}

	std::uint32_t __cdecl runtime_flags() noexcept
	{
		return g_runtime_flags.load(std::memory_order_acquire);
	}

	std::uint32_t __cdecl queue_task(
	    kcd2mp::kcse::task_callback callback,
	    void *context) noexcept
	{
		if (!callback || !g_kcse)
		{
			return 0;
		}
		auto *tasks = g_kcse->GetTaskInterface();
		if (!tasks)
		{
			return 0;
		}
		tasks->AddTask(
		    [callback, context]
		    {
			    callback(context);
		    });
		return 1;
	}

	std::uint32_t __cdecl resolve_entity(
	    std::uint32_t entity_id,
	    kcd2mp::kcse::entity_view *result) noexcept
	{
		if (!result || result->struct_size < sizeof(kcd2mp::kcse::entity_view))
		{
			return 0;
		}

		const auto environment = SSystemGlobalEnvironment::GetInstance();
		const auto context = wh::game::S_GameContext::GetInstance();
		if (!environment || !environment->pEntitySystem || !context)
		{
			return 0;
		}
		auto *entity = environment->pEntitySystem->GetEntity(entity_id);
		if (!entity)
		{
			return 0;
		}
		auto *actor = context->GetActorById(entity_id);

		result->entity_id = entity_id;
		result->entity = entity;
		result->actor = actor;
		result->soul = actor ? actor->m_pSoul : nullptr;
		return 1;
	}

	kcd2mp::kcse::bridge_api g_bridge_api{
	    sizeof(kcd2mp::kcse::bridge_api),
	    kcd2mp::kcse::bridge_abi_version,
	    0,
	    0,
	    runtime_flags,
	    queue_task,
	    resolve_entity,
	};
}

KCSE_EXPORT KCSE::PluginVersionData KCSEPlugin_Version = {
    KCSE::PluginVersionData::kDataVersion,
    1,
    "KCD2MPBridge",
    "F02K",
    {0x01050600},
    1,
    KCSE::PluginVersionData::kVersionIndependent_None,
};

KCSE_EXPORT bool KCSEPlugin_Load(const KCSE::IKCSEInterface *kcse)
{
	if (!kcse || kcse->GetKCSEVersion() < 1)
	{
		return false;
	}
	KCSE::Init(kcse);
	g_kcse = kcse;
	g_bridge_api.kcse_version = kcse->GetKCSEVersion();
	g_bridge_api.game_version = kcse->GetGameVersion();
	g_runtime_flags.store(
	    kcd2mp::kcse::runtime_plugin_loaded,
	    std::memory_order_release);
	auto *messaging = kcse->GetMessagingInterface();
	return messaging && messaging->RegisterListener("KCSE", on_kcse_message);
}

KCSE_EXPORT const kcd2mp::kcse::bridge_api *__cdecl KCD2MP_KCSE_QueryBridge(
    std::uint32_t requested_abi) noexcept
{
	return requested_abi == kcd2mp::kcse::bridge_abi_version ? &g_bridge_api :
	                                                           nullptr;
}
