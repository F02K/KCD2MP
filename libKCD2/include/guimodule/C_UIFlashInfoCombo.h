#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoCombo -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_182AFB704
// N=96 (ctor 0x182B6221C; m_groupName is built as "ui_combogroup_"+id -- literal-pinned).
// FillUIArgs override 0x182B63360 (serializes scalars + the step-id list inline).

namespace wh::guimodule {

class C_UIFlashInfoCombo : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoCombo;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B63360

    CryStringT<char> m_name;         // +0x18
    int32_t          m_int20;        // +0x20
    uint8_t          _pad24[4];      // +0x24
    CryStringT<char> m_groupName;    // +0x28  "ui_combogroup_"+id
    CryStringT<char> m_str30;        // +0x30
    std::vector<int32_t> m_steps;    // +0x38  (0x18) stride 4
};
static_assert(sizeof(C_UIFlashInfoCombo) == 0x50, "C_UIFlashInfoCombo must be 0x50 (creator sub_182AFB704)");

}  // namespace wh::guimodule
