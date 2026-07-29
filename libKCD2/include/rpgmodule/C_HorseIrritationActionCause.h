#pragma once
#include "C_Cause.h"
#include "S_HorseIrritationActionData.h"

// -----------------------------------------------
// wh::rpgmodule::C_HorseIrritationActionCause -- "HorseIrritationAction" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD5588 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5C5A0, vtable rva 0x3F27770 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x9DC14C -> visitor+0x0F0, [3] GetName rva 0x2CF5EE0 ->
// "HorseIrritationAction" (verified).  Caller sub_182CF9AB8.

namespace wh::rpgmodule {

class C_HorseIrritationActionCause : public C_Cause<S_HorseIrritationActionData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_HorseIrritationActionCause;
};
static_assert(sizeof(C_HorseIrritationActionCause) == 0x18,
              "C_HorseIrritationActionCause must be 0x18 (alloc 24 at sub_182CD5588)");

}  // namespace wh::rpgmodule
