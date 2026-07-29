#pragma once
#include "C_Cause.h"
#include "S_PowerReadingFinishedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PowerReadingFinishedCause -- "PowerReadingFinished" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_182CD5C10 tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D5CA98, vtable rva 0x3F27238 (4 slots): [0] dtor 0x1815A3D58 (ICF trivial),
// [1] nullsub, [2] Accept -> visitor+0x168, [3] GetName -> "PowerReadingFinished"
// (verified).  make+post shape; callers sub_182CFAD84 / sub_182CFD46C.

namespace wh::rpgmodule {

class C_PowerReadingFinishedCause : public C_Cause<S_PowerReadingFinishedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PowerReadingFinishedCause;
};
static_assert(sizeof(C_PowerReadingFinishedCause) == 0x20,
              "C_PowerReadingFinishedCause must be 0x20 (alloc 32 at sub_182CD5C10)");

}  // namespace wh::rpgmodule
