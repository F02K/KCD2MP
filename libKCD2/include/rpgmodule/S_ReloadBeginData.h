#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ReloadBeginData -- payload of C_ReloadBeginCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD5E20 tracked-alloc 24; _OWORD @+0x08).  Caller
// sub_182CFB5AC @0x182CFB5D1 uses *(_QWORD*)a2 as id; the high qword's source is
// decompiler-untracked (leftover register) -- present but provenance UNVERIFIED.

namespace wh::rpgmodule {

struct S_ReloadBeginData {
    uint64_t m_id;   // +0x00  id/handle [proposed]
    uint64_t m_b;    // +0x08  [U provenance]
};
static_assert(sizeof(S_ReloadBeginData) == 0x10, "S_ReloadBeginData must be 0x10");

}  // namespace wh::rpgmodule
