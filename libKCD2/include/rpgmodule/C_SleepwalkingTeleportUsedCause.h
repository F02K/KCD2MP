#pragma once
#include "C_Cause.h"
#include "S_SleepwalkingTeleportUsedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SleepwalkingTeleportUsedCause -- "SleepwalkingTeleportUsed" event
// cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_181FCE750 PLAIN-alloc sub_1804F75C0(16) -- the only
// g11e cause using the untracked allocator).  RTTI TD rva 0x4D5D3C0, vtable rva 0x3F27350
// (4 slots): [0] dtor rva 0x9DC2C0 (ICF trivial), [1] nullsub, [2] Accept rva 0x1FC04F0
// -> visitor+176, [3] GetName rva 0x1FC0A40 -> "SleepwalkingTeleportUsed" (verified).
// Factory caller sub_181FD4B90.

namespace wh::rpgmodule {

class C_SleepwalkingTeleportUsedCause : public C_Cause<S_SleepwalkingTeleportUsedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SleepwalkingTeleportUsedCause;
};
static_assert(sizeof(C_SleepwalkingTeleportUsedCause) == 0x10,
              "C_SleepwalkingTeleportUsedCause must be 0x10 (alloc 16 at sub_181FCE750)");

}  // namespace wh::rpgmodule
