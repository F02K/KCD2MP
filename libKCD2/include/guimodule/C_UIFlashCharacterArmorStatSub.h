#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCharacterArmorStatSub -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1808F60DC
// N=56 (ctor 0x1808F61B4). FillUIArgs override 0x1805570DC (two floats, inline tag-1
// builder). Element of C_UIFlashCharacterArmorStat::m_subs.

namespace wh::guimodule {

class C_UIFlashCharacterArmorStatSub : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCharacterArmorStatSub;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1805570DC

    float   m_val0;      // +0x18
    float   m_val1;      // +0x1C
    float   m_val2;      // +0x20  ctor movss same delta as m_val0/m_val1 (0x1808F6240); serialized by FillUIArgs (0x18055719F)
    uint8_t _pad24[4];   // +0x24  tail padding to 0x28 (never written)
};
static_assert(sizeof(C_UIFlashCharacterArmorStatSub) == 0x28, "C_UIFlashCharacterArmorStatSub must be 0x28 (creator sub_1808F60DC)");

}  // namespace wh::guimodule
