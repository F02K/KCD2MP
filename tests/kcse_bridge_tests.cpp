#include "kcse/bridge_api.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace
{
	std::uint32_t __cdecl flags() noexcept
	{
		return kcd2mp::kcse::runtime_plugin_loaded;
	}

	std::uint32_t __cdecl queue(
	    kcd2mp::kcse::task_callback,
	    void *) noexcept
	{
		return 1;
	}

	std::uint32_t __cdecl resolve(
	    std::uint32_t,
	    kcd2mp::kcse::entity_view *) noexcept
	{
		return 1;
	}
}

int main()
{
	using namespace kcd2mp::kcse;
	static_assert(std::is_standard_layout_v<entity_view>);
	static_assert(std::is_trivially_copyable_v<entity_view>);
	static_assert(std::is_standard_layout_v<bridge_api>);
	static_assert(std::is_trivially_copyable_v<bridge_api>);

	bridge_api valid{
	    sizeof(bridge_api),
	    bridge_abi_version,
	    1,
	    0x01050600,
	    flags,
	    queue,
	    resolve,
	};
	assert(compatible(&valid));

	auto old_abi = valid;
	old_abi.abi_version = bridge_abi_version - 1;
	assert(!compatible(&old_abi));

	auto short_struct = valid;
	short_struct.struct_size = static_cast<std::uint32_t>(
	    offsetof(bridge_api, resolve_entity));
	assert(!compatible(&short_struct));
}
