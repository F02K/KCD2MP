#include "REL/Relocation.h"

#include <Windows.h>

#include <cstring>

namespace REL
{
	void safe_write(std::uintptr_t a_dst, const void* a_src, std::size_t a_count)
	{
		DWORD old{ 0 };
		::VirtualProtect(reinterpret_cast<void*>(a_dst), a_count, PAGE_EXECUTE_READWRITE, &old);
		std::memcpy(reinterpret_cast<void*>(a_dst), a_src, a_count);
		::VirtualProtect(reinterpret_cast<void*>(a_dst), a_count, old, &old);
		::FlushInstructionCache(::GetCurrentProcess(), reinterpret_cast<void*>(a_dst), a_count);
	}

	void safe_fill(std::uintptr_t a_dst, std::uint8_t a_value, std::size_t a_count)
	{
		DWORD old{ 0 };
		::VirtualProtect(reinterpret_cast<void*>(a_dst), a_count, PAGE_EXECUTE_READWRITE, &old);
		std::memset(reinterpret_cast<void*>(a_dst), a_value, a_count);
		::VirtualProtect(reinterpret_cast<void*>(a_dst), a_count, old, &old);
		::FlushInstructionCache(::GetCurrentProcess(), reinterpret_cast<void*>(a_dst), a_count);
	}
}
