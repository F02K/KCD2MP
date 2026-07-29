#pragma once
#include "C_Cause.h"
#include "S_AimEndData.h"

// -----------------------------------------------
// wh::rpgmodule::C_AimEndCause -- "AimEnd" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: factory sub_182CD4D24 tracked-alloc 32).  RTTI TD rva 0x4D5DDB0,
// vtable rva 0x3F27678 (4 slots): [0] dtor 0x1815A3D58 (ICF trivial), [1] nullsub,
// [2] Accept 0x182C8A3EC -> visitor+704, [3] GetTypeName 0x182C8A744 -> "AimEnd"
// (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_AimEndCause : public C_Cause<S_AimEndData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AimEndCause;
};
static_assert(sizeof(C_AimEndCause) == 0x20, "C_AimEndCause must be 0x20 (alloc 32 at sub_182CD4D24)");

}  // namespace wh::rpgmodule
