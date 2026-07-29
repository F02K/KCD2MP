#pragma once
#include "C_Cause.h"
#include "S_FastTravelEndedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_FastTravelEndedCause -- "FastTravelEnded" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD5234 tracked-alloc 24, += 0x18 @0x182CD5277).
// RTTI TD rva 0x4D5D318, vtable rva 0x3F27170 (4 slots): [0] dtor rva 0x14F4A20 (ICF
// trivial, shared with FastTravelStarted), [1] nullsub, [2] Accept rva 0x12B3224 ->
// visitor+0x88, [3] GetTypeName rva 0x2CAC88C -> "FastTravelEnded" (verified).
// Pattern-B ctor; create-site sub_182CF971C.

namespace wh::rpgmodule {

class C_FastTravelEndedCause : public C_Cause<S_FastTravelEndedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FastTravelEndedCause;
};
static_assert(sizeof(C_FastTravelEndedCause) == 0x18,
              "C_FastTravelEndedCause must be 0x18 (alloc 24 at sub_182CD5234)");

}  // namespace wh::rpgmodule
