#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ItemEatData -- payload of C_ItemEatCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182CD5774 tracked-alloc 24; _OWORD @+0x08).  Caller
// sub_182CFDCD4.  Two-qword blob; fields UNVERIFIED.

namespace wh::rpgmodule {

struct S_ItemEatData {
    uint64_t m_a;   // +0x00  [U role]
    uint64_t m_b;   // +0x08  [U role]
};
static_assert(sizeof(S_ItemEatData) == 0x10, "S_ItemEatData must be 0x10");

}  // namespace wh::rpgmodule
