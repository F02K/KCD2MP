#pragma once
#include "C_Cause.h"
#include "S_MissileHitData.h"

// -----------------------------------------------
// wh::rpgmodule::C_MissileHitCause -- "MissileHit" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x90 (PROVEN: factory sub_182D226AC tracked-alloc 144, += 0x90).  RTTI TD rva
// 0x4D71A50, vtable rva 0x3F45818 (4 slots): [0] dtor rva 0x2D25770 (own body),
// [1] nullsub, [2] Accept rva 0x2CA9384 -> visitor+0x228, [3] GetName rva 0x2CACDFC ->
// "MissileHit" (verified).  Factory signature (out, srcdata, a3): returns the raw cause
// via *a1; caller sub_182D19080 wraps it in C_Event.

namespace wh::rpgmodule {

class C_MissileHitCause : public C_Cause<S_MissileHitData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MissileHitCause;
};
static_assert(sizeof(C_MissileHitCause) == 0x90, "C_MissileHitCause must be 0x90 (alloc 144 at sub_182D226AC)");

}  // namespace wh::rpgmodule
