#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SharpeningStartedData -- payload of C_SharpeningStartedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_182CD6168 tracked-alloc 24; _OWORD @+0x08).  Factory
// caller sub_182CFB970 (site 0x182CFB9B9).  16-byte POD; split UNVERIFIED -- raw blob.

namespace wh::rpgmodule {

struct S_SharpeningStartedData {
    uint8_t _raw[16];   // +0x00  [U fields]
};
static_assert(sizeof(S_SharpeningStartedData) == 0x10, "S_SharpeningStartedData must be 0x10");

}  // namespace wh::rpgmodule
