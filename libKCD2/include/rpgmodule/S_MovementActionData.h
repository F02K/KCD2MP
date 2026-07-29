#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_MovementActionData -- payload of C_MovementActionCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_180CF8CF0 tracked-alloc 24; _OWORD @+0x08).  Caller
// sub_180CF8B6C builds { *(getter), a4 } and reads +0x08 back as a switch key {0,2,3} --
// m_actionType is a VERIFIED small enum (enumerator names unknown).

namespace wh::rpgmodule {

struct S_MovementActionData {
    uint64_t m_actor;        // +0x00  actor WUID [proposed]
    int32_t  m_actionType;   // +0x08  movement-action enum {0,2,3 observed} (verified switch key)
    uint8_t  _pad0C[4];      // +0x0C
};
static_assert(sizeof(S_MovementActionData) == 0x10, "S_MovementActionData must be 0x10");

}  // namespace wh::rpgmodule
