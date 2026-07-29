#pragma once
#include "C_Cause.h"
#include "S_BlockingData.h"

// -----------------------------------------------
// wh::rpgmodule::C_BlockingCause -- "Blocking" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: factory sub_181635788 tracked-alloc 32).  RTTI TD rva 0x4D5EE68,
// vtable rva 0x3BDA460 (4 slots): [0] dtor 0x1815A3D58 (ICF trivial), [1] nullsub,
// [2] Accept 0x181954B70 -> visitor+744, [3] GetTypeName 0x182C8A7B8 -> "Blocking"
// (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_BlockingCause : public C_Cause<S_BlockingData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BlockingCause;
};
static_assert(sizeof(C_BlockingCause) == 0x20, "C_BlockingCause must be 0x20 (alloc 32 at sub_181635788)");

}  // namespace wh::rpgmodule
