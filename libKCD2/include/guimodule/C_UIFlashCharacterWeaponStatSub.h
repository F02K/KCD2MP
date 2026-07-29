#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCharacterWeaponStatSub -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1808F88C4
// N=56 (ctor 0x1808F8258). FillUIArgs override 0x180556D74 (three floats via the
// inline tag-1 builder sub_18178F0E0). Element of C_UIFlashCharacterWeaponStat::m_subs.

namespace wh::guimodule {

class C_UIFlashCharacterWeaponStatSub : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCharacterWeaponStatSub;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x180556D74

    float   m_val0;      // +0x18
    float   m_val1;      // +0x1C
    float   m_val2;      // +0x20
    uint8_t _pad24[4];   // +0x24
};
static_assert(sizeof(C_UIFlashCharacterWeaponStatSub) == 0x28, "C_UIFlashCharacterWeaponStatSub must be 0x28 (creator sub_1808F88C4)");

}  // namespace wh::guimodule
