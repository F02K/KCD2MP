#pragma once
#include "C_Cause.h"
#include "S_JailEndedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_JailEndedCause -- "JailEnded" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD5978 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5D1F8, vtable rva 0x3F27120 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x9DB4B4 -> visitor+0x098, [3] GetName rva 0x2CACD14 ->
// "JailEnded" (verified).  Caller sub_182CF9D88.

namespace wh::rpgmodule {

class C_JailEndedCause : public C_Cause<S_JailEndedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_JailEndedCause;
};
static_assert(sizeof(C_JailEndedCause) == 0x18, "C_JailEndedCause must be 0x18 (alloc 24 at sub_182CD5978)");

}  // namespace wh::rpgmodule
