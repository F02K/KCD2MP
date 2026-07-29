#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListCombo -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_18132C98C
// N=80 (ctor 0x18132CA64). FillUIArgs override 0x180556F8C (scalars + step-id list
// inline). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashListCombo : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListCombo;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x180556F8C

    CryStringT<char> m_name;       // +0x18
    int32_t          m_int20;      // +0x20
    uint8_t          _pad24[4];    // +0x24
    std::vector<int32_t> m_steps;  // +0x28  (0x18) stride 4
};
static_assert(sizeof(C_UIFlashListCombo) == 0x40, "C_UIFlashListCombo must be 0x40 (creator sub_18132C98C)");

}  // namespace wh::guimodule
