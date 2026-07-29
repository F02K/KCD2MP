#pragma once
#include "C_Cause.h"
#include "S_CompanionEventData.h"

// -----------------------------------------------
// wh::rpgmodule::C_CompanionEventCause -- "CompanionEvent" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: ctor sub_182D18E88 tracked-alloc 32, += 0x20 @0x182D18EC8).
// RTTI TD rva 0x4D71538, vtable rva 0x3F455B8 (4 slots): [0] dtor rva 0x15A3D58 (ICF
// trivial), [1] nullsub, [2] Accept rva 0xC62550 -> visitor+0x70, [3] GetTypeName rva
// 0x2C8A914 -> "CompanionEvent" (verified).  Pattern-B ctor; create-site sub_182D28FCC.

namespace wh::rpgmodule {

class C_CompanionEventCause : public C_Cause<S_CompanionEventData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CompanionEventCause;
};
static_assert(sizeof(C_CompanionEventCause) == 0x20, "C_CompanionEventCause must be 0x20 (alloc 32 at sub_182D18E88)");

}  // namespace wh::rpgmodule
