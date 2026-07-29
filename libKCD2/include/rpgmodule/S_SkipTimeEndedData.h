#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_SkipTimeEndedData -- payload of C_SkipTimeEndedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x04 (object alloc 16 = 8 vptr + 4 rounded up).  PROVEN: create fn sub_18191BC28
// dword *(v5+8)=*a3; builder sub_18191B9CC passes float a3 @0x18191BA3E -- the float type
// is VERIFIED (skipped-time hours candidate).

namespace wh::rpgmodule {

struct S_SkipTimeEndedData {
    float m_hours;   // +0x00  skipped time (float verified; unit proposed)
};
static_assert(sizeof(S_SkipTimeEndedData) == 0x04, "S_SkipTimeEndedData must be 0x04");

}  // namespace wh::rpgmodule
