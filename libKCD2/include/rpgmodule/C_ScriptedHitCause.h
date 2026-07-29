#pragma once
#include "C_Cause.h"
#include "S_ScriptedHitData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ScriptedHitCause -- "ScriptedHit" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x40 (PROVEN: create fn sub_180725A2C tracked-alloc 64, += 0x40).  RTTI TD rva
// 0x4D71AE0, vtable rva 0x3A4B348 (4 slots): [0] dtor rva 0x16BDD60, [1] nullsub,
// [2] Accept rva 0x17EBD4C -> visitor+544, [3] GetName rva 0x2CAD624 -> "ScriptedHit"
// (verified).  Factory caller sub_1807259D0.

namespace wh::rpgmodule {

class C_ScriptedHitCause : public C_Cause<S_ScriptedHitData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptedHitCause;
};
static_assert(sizeof(C_ScriptedHitCause) == 0x40, "C_ScriptedHitCause must be 0x40 (alloc 64 at sub_180725A2C)");

}  // namespace wh::rpgmodule
