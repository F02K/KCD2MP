#pragma once
#include "C_Cause.h"
#include "S_GenericAddBuffData.h"

// -----------------------------------------------
// wh::rpgmodule::C_GenericAddBuffCause -- "GenericAddBuff" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_181361E2C tracked-alloc 32, += 0x20 @0x181361E6C).
// RTTI TD rva 0x4CE62B8, vtable rva 0x3B676C0 (4 slots): [0] dtor rva 0x15A3D58 (ICF
// trivial), [1] nullsub, [2] Accept rva 0x16B4F4C -> visitor+0x290, [3] GetTypeName rva
// 0x16B5064 -> "GenericAddBuff" (verified).  Pattern-B ctor; 8 create-sites (primary
// sub_181FD1A50).

namespace wh::rpgmodule {

class C_GenericAddBuffCause : public C_Cause<S_GenericAddBuffData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GenericAddBuffCause;
};
static_assert(sizeof(C_GenericAddBuffCause) == 0x20, "C_GenericAddBuffCause must be 0x20 (alloc 32 at sub_181361E2C)");

}  // namespace wh::rpgmodule
