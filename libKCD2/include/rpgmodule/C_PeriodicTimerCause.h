#pragma once
#include "C_Cause.h"
#include "S_PeriodicTimerData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PeriodicTimerCause -- "PeriodicTimer" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// alloc 16 (PROVEN: factory sub_18150A524 tracked-alloc 16, += 0x10) but the factory
// writes ONLY the vtable -- payload region uninitialized (see S_PeriodicTimerData;
// modeled empty, so sizeof mirrors the alloc via vptr+padding).  RTTI TD rva 0x4DADB98,
// vtable rva 0x3BA7A38 (4 slots): [0] dtor rva 0x9DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept rva 0xC676E4 -> visitor+0x008, [3] GetName rva 0x165D2AC -> "PeriodicTimer"
// (verified).  Caller sub_1804C089C.

namespace wh::rpgmodule {

class C_PeriodicTimerCause : public C_Cause<S_PeriodicTimerData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PeriodicTimerCause;
};
static_assert(sizeof(C_PeriodicTimerCause) == 0x10, "C_PeriodicTimerCause must be 0x10 (alloc 16 at sub_18150A524)");

}  // namespace wh::rpgmodule
