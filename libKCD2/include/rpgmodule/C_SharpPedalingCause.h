#pragma once
#include "C_Cause.h"
#include "S_SharpPedalingData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SharpPedalingCause -- "SharpPedaling" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_182CD5F68 tracked-alloc 16).  RTTI TD rva 0x4D5E118,
// vtable rva 0x3F27468 (4 slots): [0] dtor rva 0x9DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept rva 0x2CA9410 -> visitor+472, [3] GetName rva 0x2CAD698 -> "SharpPedaling"
// (verified).  Factory caller sub_182CFD29C.

namespace wh::rpgmodule {

class C_SharpPedalingCause : public C_Cause<S_SharpPedalingData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SharpPedalingCause;
};
static_assert(sizeof(C_SharpPedalingCause) == 0x10, "C_SharpPedalingCause must be 0x10 (alloc 16 at sub_182CD5F68)");

}  // namespace wh::rpgmodule
