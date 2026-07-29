#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashListItemSelection.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListItemTrader -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88, 6 slots (ALLOC-PROVEN).
// -----------------------------------------------
// ListItem-subtree record (flash_leaves.md §3.4). Creator sub_1808F4F98 N=152 (ctor
// 0x1808F5100). FillUIArgs override 0x181689FC4 (Selection layout + one string).

namespace wh::guimodule {

class C_UIFlashListItemTrader : public C_UIFlashListItemSelection {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListItemTrader;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181689FC4

    CryStringT<char> m_str80;   // +0x80
};
static_assert(sizeof(C_UIFlashListItemTrader) == 0x88, "C_UIFlashListItemTrader must be 0x88 (creator sub_1808F4F98)");

}  // namespace wh::guimodule
