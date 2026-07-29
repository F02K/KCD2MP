#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_PotentialPlayerSneakingData -- payload of C_PotentialPlayerSneakingCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_180C5C3E4 tracked-alloc 24).  Caller sub_180C5C114
// @0x180C5C2C2: observer=*v25 (getter), sneakScore=float a2 (detection/visibility
// amount).

namespace wh::rpgmodule {

struct S_PotentialPlayerSneakingData {
    uint64_t m_observer;     // +0x00  observer handle/WUID [proposed]
    float    m_sneakScore;   // +0x08  detection/visibility amount [proposed]
    uint8_t  _pad0C[4];      // +0x0C  [U]
};
static_assert(sizeof(S_PotentialPlayerSneakingData) == 0x10, "S_PotentialPlayerSneakingData must be 0x10");

}  // namespace wh::rpgmodule
