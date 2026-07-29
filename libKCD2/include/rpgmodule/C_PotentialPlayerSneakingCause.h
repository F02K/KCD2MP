#pragma once
#include "C_Cause.h"
#include "S_PotentialPlayerSneakingData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PotentialPlayerSneakingCause -- "PotentialPlayerSneaking" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_180C5C3E4 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5F760, vtable rva 0x3A8AAD8 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial),
// [1] nullsub, [2] Accept -> visitor+0x108, [3] GetName -> "PotentialPlayerSneaking"
// (verified).  make+post shape; caller sub_180C5C114.

namespace wh::rpgmodule {

class C_PotentialPlayerSneakingCause : public C_Cause<S_PotentialPlayerSneakingData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PotentialPlayerSneakingCause;
};
static_assert(sizeof(C_PotentialPlayerSneakingCause) == 0x18,
              "C_PotentialPlayerSneakingCause must be 0x18 (alloc 24 at sub_180C5C3E4)");

}  // namespace wh::rpgmodule
