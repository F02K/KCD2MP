#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SharpeningFinishedData -- payload of C_SharpeningFinishedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x28 (PROVEN: create fn sub_182CD60B0 tracked-alloc 48; _OWORD @+8/+24 + qword
// @+40).  Factory caller sub_182CFB74C (site 0x182CFB78C).  40-byte POD; internal split
// UNVERIFIED -- raw blob.

namespace wh::rpgmodule {

struct S_SharpeningFinishedData {
    uint8_t _raw[40];   // +0x00  2x _OWORD + qword [U fields]
};
static_assert(sizeof(S_SharpeningFinishedData) == 0x28, "S_SharpeningFinishedData must be 0x28");

}  // namespace wh::rpgmodule
