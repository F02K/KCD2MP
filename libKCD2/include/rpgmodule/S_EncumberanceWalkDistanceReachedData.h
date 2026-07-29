#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_EncumberanceWalkDistanceReachedData -- payload of
// C_EncumberanceWalkDistanceReachedCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: ctor sub_182D18F38 tracked-alloc 16; v5[1]=*a3 @0x182D18F99).
// Create-site sub_180729BDC is a large distance accumulator; the copied qword's exact
// source field UNVERIFIED (tracker-entity handle candidate).

namespace wh::rpgmodule {

struct S_EncumberanceWalkDistanceReachedData {
    uint64_t m_tracker;   // +0x00  walk-distance tracker handle [U]
};
static_assert(sizeof(S_EncumberanceWalkDistanceReachedData) == 0x08,
              "S_EncumberanceWalkDistanceReachedData must be 0x08");

}  // namespace wh::rpgmodule
