#pragma once
#include "C_Cause.h"
#include "S_AthleticDistanceReachedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_AthleticDistanceReachedCause -- "AthleticDistanceReached" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_180728A4C tracked-alloc 16).  RTTI TD rva 0x4D717B0,
// vtable rva 0x3A4B828 (4 slots): [0] dtor 0x1809DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept 0x1812070AC -> visitor+344, [3] GetTypeName 0x181207260 ->
// "AthleticDistanceReached" (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_AthleticDistanceReachedCause : public C_Cause<S_AthleticDistanceReachedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AthleticDistanceReachedCause;
};
static_assert(sizeof(C_AthleticDistanceReachedCause) == 0x10,
              "C_AthleticDistanceReachedCause must be 0x10 (alloc 16 at sub_180728A4C)");

}  // namespace wh::rpgmodule
