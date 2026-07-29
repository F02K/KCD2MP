#pragma once
#include "C_Cause.h"
#include "S_ItemRepairData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemRepairCause -- "ItemRepair" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: factory sub_182CD5818 tracked-alloc 32, += 0x20).  RTTI TD rva
// 0x4D5E380, vtable rva 0x3F275B0 (4 slots): [0] dtor rva 0x15A3D58 (ICF trivial),
// [1] OVERRIDDEN -> rva 0x2CAE580 -> sub_1809155C8 (496-byte process singleton; role
// [U]), [2] Accept rva 0x2CA935C -> visitor+0x1F8, [3] GetName rva 0x2CACC2C ->
// "ItemRepair" (verified).  Callers sub_182CFE4C4 / sub_182CFE688.

namespace wh::rpgmodule {

class C_ItemRepairCause : public C_Cause<S_ItemRepairData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemRepairCause;
};
static_assert(sizeof(C_ItemRepairCause) == 0x20, "C_ItemRepairCause must be 0x20 (alloc 32 at sub_182CD5818)");

}  // namespace wh::rpgmodule
