#pragma once
#include "C_Cause.h"
#include "S_ItemDamagedOnConsciousnessLossData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemDamagedOnConsciousnessLossCause -- "ItemDamagedOnConsciousnessLoss"
// event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_181FCE6D0 PLAIN-alloc sub_1804F75C0(24) -- no
// tracked-allocator counters; one of only 3 causes using this form).  RTTI TD rva
// 0x4D5E450, vtable rva 0x3F27538 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] OVERRIDDEN -> rva 0x23CBF70 -> sub_1809155C8 (496-byte process singleton; role
// [U]), [2] Accept rva 0x1FC04D0 -> visitor+0x1E8, [3] GetName rva 0x1FC09B0 ->
// "ItemDamagedOnConsciousnessLoss" (verified).  Caller sub_181FD36A0 (via sub_181FCE7D0).

namespace wh::rpgmodule {

class C_ItemDamagedOnConsciousnessLossCause
    : public C_Cause<S_ItemDamagedOnConsciousnessLossData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemDamagedOnConsciousnessLossCause;
};
static_assert(sizeof(C_ItemDamagedOnConsciousnessLossCause) == 0x18,
              "C_ItemDamagedOnConsciousnessLossCause must be 0x18 (alloc 24 at sub_181FCE6D0)");

}  // namespace wh::rpgmodule
