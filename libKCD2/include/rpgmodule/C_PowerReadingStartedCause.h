#pragma once
#include "C_Cause.h"
#include "S_PowerReadingStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PowerReadingStartedCause -- "PowerReadingStarted" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_182CD5CC0 tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D5CBD8, vtable rva 0x3F27328 (4 slots): [0] dtor 0x1819A99AC (ICF trivial),
// [1] nullsub, [2] Accept -> visitor+0x170, [3] GetName -> "PowerReadingStarted"
// (verified).  make+post shape; caller sub_182CFA7B4.

namespace wh::rpgmodule {

class C_PowerReadingStartedCause : public C_Cause<S_PowerReadingStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PowerReadingStartedCause;
};
static_assert(sizeof(C_PowerReadingStartedCause) == 0x20,
              "C_PowerReadingStartedCause must be 0x20 (alloc 32 at sub_182CD5CC0)");

}  // namespace wh::rpgmodule
