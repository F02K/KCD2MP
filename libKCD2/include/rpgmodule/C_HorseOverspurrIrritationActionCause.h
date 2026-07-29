#pragma once
#include "C_Cause.h"
#include "S_HorseOverspurrIrritationActionData.h"

// -----------------------------------------------
// wh::rpgmodule::C_HorseOverspurrIrritationActionCause -- "HorseOverspurrIrritationAction"
// event cause (game's own spelling) (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD562C tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5C770, vtable rva 0x3F27798 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] nullsub, [2] Accept rva 0xE7C0AC -> visitor+0x0E8, [3] GetName rva 0x2CF5F54 ->
// "HorseOverspurrIrritationAction" (verified).  Caller sub_182CF9C14.

namespace wh::rpgmodule {

class C_HorseOverspurrIrritationActionCause
    : public C_Cause<S_HorseOverspurrIrritationActionData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_HorseOverspurrIrritationActionCause;
};
static_assert(sizeof(C_HorseOverspurrIrritationActionCause) == 0x18,
              "C_HorseOverspurrIrritationActionCause must be 0x18 (alloc 24 at sub_182CD562C)");

}  // namespace wh::rpgmodule
