#pragma once
#include "C_Cause.h"
#include "S_ReadingStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ReadingStartedCause -- "ReadingStarted" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_182CD5D70 tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D5CC78, vtable rva 0x3F27300 (4 slots): [0] dtor 0x1815A3D58 (ICF trivial),
// [1] nullsub, [2] Accept -> visitor+0x178, [3] GetName -> "ReadingStarted" (verified).
// make+post shape; caller sub_182CFB2AC.

namespace wh::rpgmodule {

class C_ReadingStartedCause : public C_Cause<S_ReadingStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReadingStartedCause;
};
static_assert(sizeof(C_ReadingStartedCause) == 0x20, "C_ReadingStartedCause must be 0x20 (alloc 32 at sub_182CD5D70)");

}  // namespace wh::rpgmodule
