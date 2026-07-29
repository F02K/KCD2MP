#include "kcse/bridge_client.hpp"

#include <Windows.h>

#include <atomic>

namespace kcd2mp::kcse
{
	namespace
	{
		std::atomic<const bridge_api *> g_bridge{};

		const bridge_api *load_bridge() noexcept
		{
			if (const auto *loaded = g_bridge.load(std::memory_order_acquire))
			{
				return loaded;
			}

			const auto module = GetModuleHandleW(bridge_module_name);
			if (!module)
			{
				return nullptr;
			}
			const auto query = reinterpret_cast<query_bridge>(
			    GetProcAddress(module, bridge_query_export));
			if (!query)
			{
				return nullptr;
			}
			const auto *candidate = query(bridge_abi_version);
			if (!compatible(candidate))
			{
				return nullptr;
			}
			g_bridge.store(candidate, std::memory_order_release);
			return candidate;
		}
	}

	bool refresh_bridge() noexcept
	{
		return load_bridge() != nullptr;
	}

	bridge_status current_bridge_status()
	{
		const auto *api = load_bridge();
		if (!api)
		{
			return {
			    false,
			    0,
			    0,
			    0,
			    "KCSE bridge plugin is not loaded; expected "
			    "mods/KCD2MP/KCSE/Plugins/KCD2MPKCSEBridge.dll"};
		}
		return {
		    true,
		    api->kcse_version,
		    api->game_version,
		    api->runtime_flags(),
		    "KCSE/libKCD2 bridge is ready"};
	}

	bool resolve_entity(std::uint32_t entity_id, entity_view &result) noexcept
	{
		const auto *api = load_bridge();
		if (!api)
		{
			return false;
		}
		result = {};
		result.struct_size = sizeof(result);
		return api->resolve_entity(entity_id, &result) != 0;
	}

	bool queue_task(task_callback callback, void *context) noexcept
	{
		const auto *api = load_bridge();
		return api && callback && api->queue_task(callback, context) != 0;
	}
}
