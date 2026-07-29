#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashHudQAMSlotItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1811A1FE0
// N=56 (ctor 0x1811A20B0). FillUIArgs override 0x1815AB9A8 (shared base+str+int body).
// Element of C_UIFlashHudQAMSlot::m_items.

namespace wh::guimodule {

class C_UIFlashHudQAMSlotItem : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashHudQAMSlotItem;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1815AB9A8 (shared body)

    CryStringT<char> m_icon;     // +0x18
    int32_t          m_int20;    // +0x20
    uint8_t          _pad24[4];  // +0x24
};
static_assert(sizeof(C_UIFlashHudQAMSlotItem) == 0x28, "C_UIFlashHudQAMSlotItem must be 0x28 (creator sub_1811A1FE0)");

}  // namespace wh::guimodule
