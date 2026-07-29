#pragma once
#include "C_Cause.h"
#include "S_SharpPressureData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SharpPressureCause -- "SharpPressure" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: create fn sub_182CD600C tracked-alloc 24).  RTTI TD rva 0x4D5E2F8,
// vtable rva 0x3F27490 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial), [1] nullsub,
// [2] Accept rva 0x2CA9424 -> visitor+464, [3] GetName rva 0x2CAD70C -> "SharpPressure"
// (verified).  Factory caller sub_182CFD360.

namespace wh::rpgmodule {

class C_SharpPressureCause : public C_Cause<S_SharpPressureData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SharpPressureCause;
};
static_assert(sizeof(C_SharpPressureCause) == 0x18, "C_SharpPressureCause must be 0x18 (alloc 24 at sub_182CD600C)");

}  // namespace wh::rpgmodule
