#pragma once

#include <cstddef>
#include <cstdint>

namespace kcd2mp::kcse
{
	inline constexpr std::uint32_t bridge_abi_version = 1;
	inline constexpr wchar_t bridge_module_name[] = L"KCD2MPKCSEBridge.dll";
	inline constexpr char bridge_query_export[] = "KCD2MP_KCSE_QueryBridge";

	enum runtime_flag : std::uint32_t
	{
		runtime_plugin_loaded = 1U << 0,
		runtime_all_plugins_loaded = 1U << 1,
		runtime_data_loaded = 1U << 2,
		runtime_save_loaded = 1U << 3,
	};

	using task_callback = void(__cdecl *)(void *context) noexcept;

	struct entity_view
	{
		std::uint32_t struct_size{sizeof(entity_view)};
		std::uint32_t entity_id{};
		void *entity{};
		void *actor{};
		void *soul{};
	};

	struct bridge_api
	{
		std::uint32_t struct_size{};
		std::uint32_t abi_version{};
		std::uint32_t kcse_version{};
		std::uint32_t game_version{};
		std::uint32_t(__cdecl *runtime_flags)() noexcept{};
		std::uint32_t(__cdecl *queue_task)(task_callback callback, void *context) noexcept{};
		std::uint32_t(__cdecl *resolve_entity)(std::uint32_t entity_id, entity_view *result) noexcept{};
	};

	using query_bridge = const bridge_api *(__cdecl *)(std::uint32_t requested_abi) noexcept;

	[[nodiscard]] constexpr bool compatible(const bridge_api *api) noexcept
	{
		return api && api->struct_size >= sizeof(bridge_api)
		    && api->abi_version == bridge_abi_version && api->runtime_flags
		    && api->queue_task && api->resolve_entity;
	}
}
