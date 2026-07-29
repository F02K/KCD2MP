#pragma once
#include "C_Cause.h"
#include "S_GenericRemoveBuffsData.h"

// -----------------------------------------------
// wh::rpgmodule::C_GenericRemoveBuffsCause -- "GenericRemoveBuffs" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_181361ED8 tracked-alloc 16, += 0x10).  RTTI TD rva
// 0x4CE9920, vtable rva 0x3B67710 (4 slots): [0] dtor rva 0x9DC2C0 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x1611180 -> visitor+0x298, [3] GetName rva 0x1611194 ->
// "GenericRemoveBuffs" (verified).  Callers sub_181FCE7D0 / sub_182BDB584 / sub_182CFC978.

namespace wh::rpgmodule {

class C_GenericRemoveBuffsCause : public C_Cause<S_GenericRemoveBuffsData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GenericRemoveBuffsCause;
};
static_assert(sizeof(C_GenericRemoveBuffsCause) == 0x10,
              "C_GenericRemoveBuffsCause must be 0x10 (alloc 16 at sub_181361ED8)");

}  // namespace wh::rpgmodule
