#pragma once
#include <string_view>

// -----------------------------------------------
// rttr::string_view -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Upstream rttr ships its own rttr::basic_string_view<char> = {const char* m_data,
// size_t m_length}. MSVC std::string_view has the identical {ptr, len} layout --
// binary-verified at type_data+0x38/+0x40: every creator stores the __FUNCSIG__-slice
// pointer at +0x38 and the length immediate at +0x40 (e.g. C_UIBase 0x1816391F4:
// len 0x1D = strlen("class wh::guimodule::C_UIBase")).
// Per project rule (prefer std types over re-spelling), alias instead of re-defining.

namespace rttr {
using string_view = std::string_view;
}  // namespace rttr

static_assert(sizeof(rttr::string_view) == 0x10, "string_view must be {ptr, len}");
