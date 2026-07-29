#pragma once
#include "C_Cause.h"
#include "S_ReloadEndData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ReloadEndCause -- "ReloadEnd" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD5EC4 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5DA58, vtable rva 0x3F275D8 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial),
// [1] OVERRIDDEN -> 0x1823CBF70 -> sub_1809155C8 (496-byte process singleton; role [U]),
// [2] Accept -> visitor+0x2B0, [3] GetName -> "ReloadEnd" (verified).  make+post shape;
// caller sub_182CFB6E0.

namespace wh::rpgmodule {

class C_ReloadEndCause : public C_Cause<S_ReloadEndData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReloadEndCause;
};
static_assert(sizeof(C_ReloadEndCause) == 0x18, "C_ReloadEndCause must be 0x18 (alloc 24 at sub_182CD5EC4)");

}  // namespace wh::rpgmodule
