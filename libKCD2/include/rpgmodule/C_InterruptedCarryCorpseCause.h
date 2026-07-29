#pragma once
#include "C_Cause.h"
#include "S_InterruptedCarryCorpseData.h"

// -----------------------------------------------
// wh::rpgmodule::C_InterruptedCarryCorpseCause -- "InterruptedCarryCorpse" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD56D0 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5E590, vtable rva 0x3F27600 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x2CA9334 -> visitor+0x218, [3] GetName rva 0x2CACAD0 ->
// "InterruptedCarryCorpse" (verified).  Caller sub_182CF9D2C.

namespace wh::rpgmodule {

class C_InterruptedCarryCorpseCause : public C_Cause<S_InterruptedCarryCorpseData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InterruptedCarryCorpseCause;
};
static_assert(sizeof(C_InterruptedCarryCorpseCause) == 0x18,
              "C_InterruptedCarryCorpseCause must be 0x18 (alloc 24 at sub_182CD56D0)");

}  // namespace wh::rpgmodule
