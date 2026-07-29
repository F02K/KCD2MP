#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashListItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListItemSelection -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x80, 6 slots (ALLOC-PROVEN).
// -----------------------------------------------
// ListItem-subtree record (flash_leaves.md §3.4). Creator sub_1808F4D5C N=144 (ctor
// 0x1808F5FB8). FillUIArgs override 0x181706F6C (ListItem base + one int).

namespace wh::guimodule {

class C_UIFlashListItemSelection : public C_UIFlashListItem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListItemSelection;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181706F6C

    int32_t m_int78;     // +0x78  raw tag-0 append
    uint8_t _pad7C[4];   // +0x7C
};
static_assert(sizeof(C_UIFlashListItemSelection) == 0x80, "C_UIFlashListItemSelection must be 0x80 (creator sub_1808F4D5C)");

}  // namespace wh::guimodule
