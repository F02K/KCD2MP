#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashListItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListItemApse -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x80, 6 slots (ALLOC-PROVEN).
// -----------------------------------------------
// ListItem-subtree record (flash_leaves.md §3.4). Creator sub_1808F6664 N=144 (ctor
// 0x1808F6700). FillUIArgs override 0x18162038C (ListItem base + one string).

namespace wh::guimodule {

class C_UIFlashListItemApse : public C_UIFlashListItem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListItemApse;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x18162038C

    CryStringT<char> m_str78;   // +0x78
};
static_assert(sizeof(C_UIFlashListItemApse) == 0x80, "C_UIFlashListItemApse must be 0x80 (creator sub_1808F6664)");

}  // namespace wh::guimodule
