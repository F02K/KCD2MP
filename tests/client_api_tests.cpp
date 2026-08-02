#include "kcse/client_api.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace
{
	std::uint32_t __cdecl runtime(
	    kcd2mp::kcse::runtime_status *) noexcept
	{
		return 1;
	}
	std::uint32_t __cdecl connect(
	    const kcd2mp::kcse::connect_request *) noexcept
	{
		return 1;
	}
	void __cdecl disconnect() noexcept {}
	std::uint32_t __cdecl text(const char *) noexcept { return 1; }
	std::uint32_t __cdecl status(
	    kcd2mp::kcse::client_status_view *) noexcept
	{
		return 1;
	}
	std::uint32_t __cdecl players(
	    kcd2mp::kcse::remote_player_view *,
	    std::uint32_t) noexcept
	{
		return 0;
	}
	std::uint32_t __cdecl chat(
	    kcd2mp::kcse::chat_entry_view *,
	    std::uint32_t) noexcept
	{
		return 0;
	}
	std::uint32_t __cdecl archetypes(
	    kcd2mp::kcse::fixed_string *,
	    std::uint32_t) noexcept
	{
		return 0;
	}
	void __cdecl diagnostic_logging(std::uint32_t) noexcept {}
}

int main()
{
	using namespace kcd2mp::kcse;
	static_assert(std::is_standard_layout_v<fixed_string>);
	static_assert(std::is_trivially_copyable_v<fixed_string>);
	static_assert(std::is_standard_layout_v<connect_request>);
	static_assert(std::is_trivially_copyable_v<connect_request>);
	static_assert(std::is_standard_layout_v<runtime_status>);
	static_assert(std::is_trivially_copyable_v<runtime_status>);
	static_assert(std::is_standard_layout_v<client_status_view>);
	static_assert(std::is_trivially_copyable_v<client_status_view>);
	static_assert(std::is_standard_layout_v<remote_player_view>);
	static_assert(std::is_trivially_copyable_v<remote_player_view>);
	static_assert(std::is_standard_layout_v<chat_entry_view>);
	static_assert(std::is_trivially_copyable_v<chat_entry_view>);
	static_assert(std::is_standard_layout_v<client_api>);
	static_assert(std::is_trivially_copyable_v<client_api>);
#ifdef _WIN64
	static_assert(sizeof(fixed_string) == 64);
	static_assert(sizeof(connect_request) == 516);
	static_assert(sizeof(runtime_status) == 424);
	static_assert(sizeof(client_status_view) == 672);
	static_assert(sizeof(remote_player_view) == 80);
	static_assert(sizeof(chat_entry_view) == 336);
	static_assert(sizeof(client_api) == 96);
#endif

	client_api valid{
	    sizeof(client_api),
	    client_abi_version,
	    client_build_id,
	    runtime,
	    connect,
	    disconnect,
	    text,
	    text,
	    status,
	    players,
	    chat,
	    archetypes,
	    diagnostic_logging};
	assert(compatible(&valid));

	auto old_abi = valid;
	old_abi.abi_version = client_abi_version - 1;
	assert(!compatible(&old_abi));

	auto wrong_build = valid;
	++wrong_build.build_id;
	assert(!compatible(&wrong_build));

	auto short_struct = valid;
	short_struct.struct_size =
	    static_cast<std::uint32_t>(offsetof(client_api, copy_chat));
	assert(!compatible(&short_struct));

	auto future_struct = valid;
	++future_struct.struct_size;
	assert(!compatible(&future_struct));

	auto missing_function = valid;
	missing_function.copy_players = nullptr;
	assert(!compatible(&missing_function));
}
