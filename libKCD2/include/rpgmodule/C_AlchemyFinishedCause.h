#pragma once
#include "C_Cause.h"
#include "S_AlchemyFinishedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_AlchemyFinishedCause -- "AlchemyFinished" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_1819357BC tracked-alloc 16).  RTTI TD rva 0x4D5CE58,
// vtable rva 0x3C3A3A0 (4 slots): [0] dtor 0x1809DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept 0x1819B0554 -> visitor+392, [3] GetTypeName 0x1819B0568 -> "AlchemyFinished"
// (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_AlchemyFinishedCause : public C_Cause<S_AlchemyFinishedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyFinishedCause;
};
static_assert(sizeof(C_AlchemyFinishedCause) == 0x10, "C_AlchemyFinishedCause must be 0x10 (alloc 16 at sub_1819357BC)");

}  // namespace wh::rpgmodule
