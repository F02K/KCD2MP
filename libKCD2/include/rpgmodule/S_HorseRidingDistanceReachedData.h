#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_HorseRidingDistanceReachedData -- payload of
// C_HorseRidingDistanceReachedCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_18192C2C4 tracked-alloc 16; v5[1]=*a3).  Caller
// sub_180729BDC (distance accumulator).  Distance vs actor-handle semantics UNVERIFIED.

namespace wh::rpgmodule {

struct S_HorseRidingDistanceReachedData {
    uint64_t m_value;   // +0x00  distance or actor handle [U]
};
static_assert(sizeof(S_HorseRidingDistanceReachedData) == 0x08,
              "S_HorseRidingDistanceReachedData must be 0x08");

}  // namespace wh::rpgmodule
