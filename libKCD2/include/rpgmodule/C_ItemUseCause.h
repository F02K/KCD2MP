#pragma once
#include "C_Cause.h"
#include "S_ItemUseData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemUseCause -- "ItemUse" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: factory sub_182CD58C8 tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D5E410, vtable rva 0x3F27588 (4 slots): [0] dtor rva 0x15A3D58 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x2CA9370 -> visitor+0x200, [3] GetName rva 0x2CACCA0 ->
// "ItemUse" (verified).  Callers sub_1823D2C30 / sub_182CFC2AC / sub_182CFE590.

namespace wh::rpgmodule {

class C_ItemUseCause : public C_Cause<S_ItemUseData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemUseCause;
};
static_assert(sizeof(C_ItemUseCause) == 0x20, "C_ItemUseCause must be 0x20 (alloc 32 at sub_182CD58C8)");

}  // namespace wh::rpgmodule
