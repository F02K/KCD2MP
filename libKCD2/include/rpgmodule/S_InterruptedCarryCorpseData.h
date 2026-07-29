#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_InterruptedCarryCorpseData -- payload of C_InterruptedCarryCorpseCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182CD56D0 tracked-alloc 24; _OWORD @+0x08).  Caller
// sub_182CF9D2C reuses *(a3+8) downstream.  Two-handle split proposed.

namespace wh::rpgmodule {

struct S_InterruptedCarryCorpseData {
    uint64_t m_a;   // +0x00  handle [proposed]
    uint64_t m_b;   // +0x08  handle [proposed]
};
static_assert(sizeof(S_InterruptedCarryCorpseData) == 0x10, "S_InterruptedCarryCorpseData must be 0x10");

}  // namespace wh::rpgmodule
