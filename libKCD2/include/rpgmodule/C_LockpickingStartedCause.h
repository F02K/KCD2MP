#pragma once
#include "C_Cause.h"
#include "S_LockpickingStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_LockpickingStartedCause -- "LockpickingStarted" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_18193569C tracked-alloc 16, += 0x10).  RTTI TD rva
// 0x4D5CDB8, vtable rva 0x3C3A350 (4 slots): [0] dtor rva 0x9DC2C0 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x19B0444 -> visitor+0x1A0, [3] GetName rva 0x19B0458 ->
// "LockpickingStarted" (verified).  Caller sub_18193561C.

namespace wh::rpgmodule {

class C_LockpickingStartedCause : public C_Cause<S_LockpickingStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LockpickingStartedCause;
};
static_assert(sizeof(C_LockpickingStartedCause) == 0x10,
              "C_LockpickingStartedCause must be 0x10 (alloc 16 at sub_18193569C)");

}  // namespace wh::rpgmodule
