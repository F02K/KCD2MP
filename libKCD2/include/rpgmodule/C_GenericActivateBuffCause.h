#pragma once
#include "C_Cause.h"
#include "S_GenericActivateBuffData.h"

// -----------------------------------------------
// wh::rpgmodule::C_GenericActivateBuffCause -- "GenericActivateBuff" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_181056068 tracked-alloc 32, += 0x20 @0x1810560A7).
// RTTI TD rva 0x4CE4058, vtable rva 0x3AF4CC0 (4 slots): [0] dtor rva 0x15A3D58 (ICF
// trivial), [1] nullsub, [2] Accept rva 0x17336AC -> visitor+0x288, [3] GetTypeName rva
// 0x17336C0 -> "GenericActivateBuff" (verified).  Pattern-B ctor; create-sites
// sub_181055EF4 / sub_18046DD90.

namespace wh::rpgmodule {

class C_GenericActivateBuffCause : public C_Cause<S_GenericActivateBuffData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GenericActivateBuffCause;
};
static_assert(sizeof(C_GenericActivateBuffCause) == 0x20,
              "C_GenericActivateBuffCause must be 0x20 (alloc 32 at sub_181056068)");

}  // namespace wh::rpgmodule
