#pragma once
#include "C_Cause.h"
#include "S_ItemEatData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemEatCause -- "ItemEat" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD5774 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5E550, vtable rva 0x3F27560 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x2CA9348 -> visitor+0x208, [3] GetName rva 0x2CACBB8 ->
// "ItemEat" (verified).  Caller sub_182CFDCD4.

namespace wh::rpgmodule {

class C_ItemEatCause : public C_Cause<S_ItemEatData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemEatCause;
};
static_assert(sizeof(C_ItemEatCause) == 0x18, "C_ItemEatCause must be 0x18 (alloc 24 at sub_182CD5774)");

}  // namespace wh::rpgmodule
