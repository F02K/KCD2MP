#pragma once
#include "C_Cause.h"
#include "S_BowDrawEndData.h"

// -----------------------------------------------
// wh::rpgmodule::C_BowDrawEndCause -- "BowDrawEnd" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182CD4EE0 tracked-alloc 16).  RTTI TD rva 0x4D5DC90,
// vtable rva 0x3F27718 (4 slots): [0] dtor 0x1809DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept 0x182C8A414 -> visitor+720, [3] GetTypeName 0x182C8A8A0 -> "BowDrawEnd"
// (verified).  Event-factory shape.  (vtable+0x20 = 0xffffffff is .rdata alignment
// padding, not a 5th slot.)

namespace wh::rpgmodule {

class C_BowDrawEndCause : public C_Cause<S_BowDrawEndData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BowDrawEndCause;
};
static_assert(sizeof(C_BowDrawEndCause) == 0x10, "C_BowDrawEndCause must be 0x10 (alloc 16 at sub_182CD4EE0)");

}  // namespace wh::rpgmodule
