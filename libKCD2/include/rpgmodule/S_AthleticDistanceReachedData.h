#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_AthleticDistanceReachedData -- payload of C_AthleticDistanceReachedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_180728A4C tracked-alloc 16; one qword to cause+0x08).
// The qword could be a packed distance/id rather than a WUID -- producer not traced [U].

namespace wh::rpgmodule {

struct S_AthleticDistanceReachedData {
    uint64_t m_value;   // +0x00  WUID or packed distance/id [U]
};
static_assert(sizeof(S_AthleticDistanceReachedData) == 0x08, "S_AthleticDistanceReachedData must be 0x08");

}  // namespace wh::rpgmodule
