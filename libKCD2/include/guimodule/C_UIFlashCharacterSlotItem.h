#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCharacterSlotItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1808F7DC0
// N=64 (ctor 0x1808F7E88). FillUIArgs override 0x1805557D0. Element of
// C_UIFlashCharacterSlot::m_items (paperdoll slot item). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashCharacterSlotItem : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCharacterSlotItem;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1805557D0

    CryStringT<char> m_icon;        // +0x18
    int32_t          m_amount;      // +0x20
    float            m_condition;   // +0x24
    float            m_quality;     // +0x28
    int32_t          m_flags;       // +0x2C  [may be an enum/bitset]
};
static_assert(sizeof(C_UIFlashCharacterSlotItem) == 0x30, "C_UIFlashCharacterSlotItem must be 0x30 (creator sub_1808F7DC0)");

}  // namespace wh::guimodule
