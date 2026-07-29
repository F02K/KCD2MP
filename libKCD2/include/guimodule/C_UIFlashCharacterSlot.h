#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "guimodule/C_UIFlashCharacterSlotItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCharacterSlot -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_1808F5470
// N=80 (ctor 0x1808F5520). FillUIArgs override 0x180555834 (own scalars); child vector
// serialized by the _vf2 [2] override 0x180556AC4. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashCharacterSlot : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCharacterSlot;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x180555834
    void _vf2(void* pArgs) const override;         // [2] 0x180556AC4  serializes m_items

    int32_t m_int18;      // +0x18
    bool    m_bool1C;     // +0x1C
    uint8_t _pad1D[3];    // +0x1D
    int32_t m_int20;      // +0x20
    int32_t m_int24;      // +0x24
    std::vector<std::shared_ptr<C_UIFlashCharacterSlotItem>> m_items;   // +0x28  (0x18)
};
static_assert(sizeof(C_UIFlashCharacterSlot) == 0x40, "C_UIFlashCharacterSlot must be 0x40 (creator sub_1808F5470)");
static_assert(offsetof(C_UIFlashCharacterSlot, m_items) == 0x28, "item vector at 0x28");

}  // namespace wh::guimodule
