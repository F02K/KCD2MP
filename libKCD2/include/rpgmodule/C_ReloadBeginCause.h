#pragma once
#include "C_Cause.h"
#include "S_ReloadBeginData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ReloadBeginCause -- "ReloadBegin" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD5E20 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5DD20, vtable rva 0x3F276C8 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial),
// [1] OVERRIDDEN -> 0x1823CBF70 -> sub_1809155C8 (496-byte process singleton; role [U]),
// [2] Accept -> visitor+0x2B8, [3] GetName -> "ReloadBegin" (verified).  make+post shape;
// caller sub_182CFB5AC.

namespace wh::rpgmodule {

class C_ReloadBeginCause : public C_Cause<S_ReloadBeginData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ReloadBeginCause;
};
static_assert(sizeof(C_ReloadBeginCause) == 0x18, "C_ReloadBeginCause must be 0x18 (alloc 24 at sub_182CD5E20)");

}  // namespace wh::rpgmodule
