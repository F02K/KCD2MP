#pragma once
#include "C_Cause.h"
#include "S_KilledByTimerData.h"

// -----------------------------------------------
// wh::rpgmodule::C_KilledByTimerCause -- "KilledByTimer" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182D18FDC tracked-alloc 16, += 0x10).  RTTI TD rva
// 0x4D71FE8, vtable rva 0x3F45888 (4 slots): [0] dtor rva 0x9DC2C0 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x9DC188 -> visitor+0x0F8, [3] GetName rva 0x2D27DE0 ->
// "KilledByTimer" (verified).  Caller sub_182D28A7C.

namespace wh::rpgmodule {

class C_KilledByTimerCause : public C_Cause<S_KilledByTimerData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_KilledByTimerCause;
};
static_assert(sizeof(C_KilledByTimerCause) == 0x10, "C_KilledByTimerCause must be 0x10 (alloc 16 at sub_182D18FDC)");

}  // namespace wh::rpgmodule
