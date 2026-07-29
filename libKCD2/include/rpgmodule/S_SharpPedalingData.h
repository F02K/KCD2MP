#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SharpPedalingData -- payload of C_SharpPedalingCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: create fn sub_182CD5F68 tracked-alloc 16; v5[1]=*a3 @0x182CD5FC9).
// Factory caller sub_182CFD29C (site 0x182CFD2F1).

namespace wh::rpgmodule {

struct S_SharpPedalingData {
    uint64_t m_value;   // +0x00  handle/WUID or value [U]
};
static_assert(sizeof(S_SharpPedalingData) == 0x08, "S_SharpPedalingData must be 0x08");

}  // namespace wh::rpgmodule
