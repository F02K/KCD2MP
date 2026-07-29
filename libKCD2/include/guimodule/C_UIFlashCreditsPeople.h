#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCreditsPeople -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1/§3.2). Creator sub_182B5F690
// N=48 (ctor 0x182B61B64 sets m_roleId = *(src+0x14) -- the S_CreditsPeople
// credit_role_id). FillUIArgs [1] = base thunk 0x181851FE8; m_roleId is NOT
// flash-serialized -- it is the grouping key C_UIFlashCreditsRole buckets by.

namespace wh::guimodule {

class C_UIFlashCreditsPeople : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCreditsPeople;
    int32_t m_roleId;    // +0x18  grouping key (S_CreditsPeople::credit_role_id)
    uint8_t _pad1C[4];   // +0x1C
};
static_assert(sizeof(C_UIFlashCreditsPeople) == 0x20, "C_UIFlashCreditsPeople must be 0x20 (creator sub_182B5F690)");

}  // namespace wh::guimodule
