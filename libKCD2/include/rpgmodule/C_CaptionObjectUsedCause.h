#pragma once
#include "C_Cause.h"
#include "S_CaptionObjectUsedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_CaptionObjectUsedCause -- "CaptionObjectUsed" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: factory sub_182CD4F84 tracked-alloc 32).  RTTI TD rva 0x4D5D5A0,
// vtable rva 0x3F27378 (4 slots): [0] dtor 0x1815A3D58 (ICF trivial), [1] nullsub,
// [2] Accept 0x180C639A4 -> visitor+168, [3] GetTypeName 0x182CAC5D4 ->
// "CaptionObjectUsed" (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_CaptionObjectUsedCause : public C_Cause<S_CaptionObjectUsedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CaptionObjectUsedCause;
};
static_assert(sizeof(C_CaptionObjectUsedCause) == 0x20,
              "C_CaptionObjectUsedCause must be 0x20 (alloc 32 at sub_182CD4F84)");

}  // namespace wh::rpgmodule
