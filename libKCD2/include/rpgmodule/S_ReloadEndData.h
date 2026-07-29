#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ReloadEndData -- payload of C_ReloadEndCause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD5EC4 tracked-alloc 24; same layout/shape as
// S_ReloadBeginData).  Caller sub_182CFB6E0 @0x182CFB705.

namespace wh::rpgmodule {

struct S_ReloadEndData {
    uint64_t m_id;   // +0x00  id/handle [proposed]
    uint64_t m_b;    // +0x08  [U provenance]
};
static_assert(sizeof(S_ReloadEndData) == 0x10, "S_ReloadEndData must be 0x10");

}  // namespace wh::rpgmodule
