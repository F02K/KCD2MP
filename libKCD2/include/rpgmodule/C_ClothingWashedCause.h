#pragma once
#include "C_Cause.h"
#include "S_ClothingWashedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ClothingWashedCause -- "ClothingWashed" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_1809DCAAC tracked-alloc 16).  RTTI TD rva 0x4D5D4F8,
// vtable rva 0x3A68CF0 (4 slots): [0] dtor 0x1809DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept 0x1809DB474 -> visitor+160, [3] GetTypeName 0x1819BFEDC -> "ClothingWashed"
// (verified).  Event-factory shape; create-site sub_1809DC9D8 documents the full
// cause -> C_Event -> enrich -> post funnel.

namespace wh::rpgmodule {

class C_ClothingWashedCause : public C_Cause<S_ClothingWashedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ClothingWashedCause;
};
static_assert(sizeof(C_ClothingWashedCause) == 0x10, "C_ClothingWashedCause must be 0x10 (alloc 16 at sub_1809DCAAC)");

}  // namespace wh::rpgmodule
