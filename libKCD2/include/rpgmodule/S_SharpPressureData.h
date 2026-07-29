#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SharpPressureData -- payload of C_SharpPressureCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_182CD600C tracked-alloc 24; _OWORD @+0x08).  Factory
// caller sub_182CFD360 (site 0x182CFD3D2).  16-byte POD; split UNVERIFIED (two qwords or
// qword + 2x float candidates) -- raw blob.

namespace wh::rpgmodule {

struct S_SharpPressureData {
    uint8_t _raw[16];   // +0x00  [U fields]
};
static_assert(sizeof(S_SharpPressureData) == 0x10, "S_SharpPressureData must be 0x10");

}  // namespace wh::rpgmodule
