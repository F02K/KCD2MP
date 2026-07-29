#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ScriptedHitData -- payload of C_ScriptedHitCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x38 (PROVEN: create fn sub_180725A2C tracked-alloc 64, += 0x40; copies _OWORD
// @+8/+24/+40 + qword @+56).  56-byte POD block; internal boundaries UNVERIFIED (likely
// attacker/victim WUIDs + hit vec/damage) -- raw blob.

namespace wh::rpgmodule {

struct S_ScriptedHitData {
    uint8_t _raw[56];   // +0x00  3x _OWORD + qword [U fields]
};
static_assert(sizeof(S_ScriptedHitData) == 0x38, "S_ScriptedHitData must be 0x38");

}  // namespace wh::rpgmodule
