#pragma once
#include "C_Cause.h"
#include "S_AimBeginData.h"

// -----------------------------------------------
// wh::rpgmodule::C_AimBeginCause -- "AimBegin" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: factory sub_182CD4C74 tracked-alloc 32).  RTTI TD rva 0x4D5DC00,
// vtable rva 0x3F27650 (4 slots): [0] dtor 0x1815A3D58 (ICF trivial), [1] OVERRIDDEN ->
// 0x1823CBF70 -> sub_1809155C8 (496-byte process singleton; role [U] -- one of only 6
// causes overriding slot 1), [2] Accept 0x182C8A3D8 -> visitor+712, [3] GetTypeName
// 0x182C8A6D0 -> "AimBegin" (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_AimBeginCause : public C_Cause<S_AimBeginData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AimBeginCause;
};
static_assert(sizeof(C_AimBeginCause) == 0x20, "C_AimBeginCause must be 0x20 (alloc 32 at sub_182CD4C74)");

}  // namespace wh::rpgmodule
