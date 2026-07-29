#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "guimodule/C_UIFlashHudQAMSlotItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashHudQAMSlot -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_1811A1C98
// N=72 (ctor 0x1811A1D9C; slot state values seen 1/2/3). FillUIArgs [1] 0x1816829DC
// (shared base+int body); item vector serialized by the _vf2 [2] override.

namespace wh::guimodule {

class C_UIFlashHudQAMSlot : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashHudQAMSlot;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1816829DC (shared base+int body)
    void _vf2(void* pArgs) const override;         // [2] serializes m_items

    int32_t m_state;     // +0x18  slot state (1/2/3 observed) [may be an enum]
    uint8_t _pad1C[4];   // +0x1C
    std::vector<std::shared_ptr<C_UIFlashHudQAMSlotItem>> m_items;   // +0x20  (0x18)
};
static_assert(sizeof(C_UIFlashHudQAMSlot) == 0x38, "C_UIFlashHudQAMSlot must be 0x38 (creator sub_1811A1C98)");

}  // namespace wh::guimodule
