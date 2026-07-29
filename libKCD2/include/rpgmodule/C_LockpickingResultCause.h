#pragma once
#include "C_Cause.h"
#include "S_LockpickingResultData.h"

// -----------------------------------------------
// wh::rpgmodule::C_LockpickingResultCause -- "LockpickingResult" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_180898A9C tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5CD18, vtable rva 0x3A5A5D0 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] nullsub, [2] Accept rva 0xE7B258 -> visitor+0x198, [3] GetName rva 0xE7B420 ->
// "LockpickingResult" (verified).  Caller sub_18089832C.

namespace wh::rpgmodule {

class C_LockpickingResultCause : public C_Cause<S_LockpickingResultData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LockpickingResultCause;
};
static_assert(sizeof(C_LockpickingResultCause) == 0x18,
              "C_LockpickingResultCause must be 0x18 (alloc 24 at sub_180898A9C)");

}  // namespace wh::rpgmodule
