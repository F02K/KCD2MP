#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashListItemSelection.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListItemSelectionCost -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88, 6 slots (ALLOC-PROVEN).
// -----------------------------------------------
// ListItem-subtree record (flash_leaves.md §3.4). Creator sub_182B0C980 N=152 (ctor
// 0x182B71688). FillUIArgs override 0x182B71B88 (Selection layout + the cost float).

namespace wh::guimodule {

class C_UIFlashListItemSelectionCost : public C_UIFlashListItemSelection {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListItemSelectionCost;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B71B88

    float   m_cost;      // +0x80
    uint8_t _pad84[4];   // +0x84
};
static_assert(sizeof(C_UIFlashListItemSelectionCost) == 0x88, "C_UIFlashListItemSelectionCost must be 0x88 (creator sub_182B0C980)");

}  // namespace wh::guimodule
