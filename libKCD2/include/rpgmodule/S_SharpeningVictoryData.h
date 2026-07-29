#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SharpeningVictoryData -- payload of C_SharpeningVictoryCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_182CD620C tracked-alloc 24; _OWORD @+0x08).  Factory
// caller sub_182CF19E0 (site 0x182CF1AD3).  16-byte POD; split UNVERIFIED -- raw blob.

namespace wh::rpgmodule {

struct S_SharpeningVictoryData {
    uint8_t _raw[16];   // +0x00  [U fields]
};
static_assert(sizeof(S_SharpeningVictoryData) == 0x10, "S_SharpeningVictoryData must be 0x10");

}  // namespace wh::rpgmodule
