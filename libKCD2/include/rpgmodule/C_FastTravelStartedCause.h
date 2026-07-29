#pragma once
#include "C_Cause.h"
#include "S_FastTravelStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_FastTravelStartedCause -- "FastTravelStarted" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD52E8 tracked-alloc 24, += 0x18 @0x182CD532B).
// RTTI TD rva 0x4D5D160, vtable rva 0x3F27148 (4 slots): [0] dtor rva 0x14F4A20 (ICF
// trivial, shared with FastTravelEnded), [1] nullsub, [2] Accept rva 0x18AC79C ->
// visitor+0x90, [3] GetTypeName rva 0x2CAC900 -> "FastTravelStarted" (verified).
// Pattern-B ctor; create-site sub_182CF9824 (refs "RPGMinigames").

namespace wh::rpgmodule {

class C_FastTravelStartedCause : public C_Cause<S_FastTravelStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FastTravelStartedCause;
};
static_assert(sizeof(C_FastTravelStartedCause) == 0x18,
              "C_FastTravelStartedCause must be 0x18 (alloc 24 at sub_182CD52E8)");

}  // namespace wh::rpgmodule
