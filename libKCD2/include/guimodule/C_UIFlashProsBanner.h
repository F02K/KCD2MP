#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashProsBanner -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_181782924
// N=80. FillUIArgs override 0x1819A2598 (COMDAT-shared with C_UIFlashContentRecipeHerb).
// ctor 0x1817829D4 fills the strings with hardcoded PlayStation-rewards literals:
// "ui_menu_prosAccount_heading", "ui_pros_banner_Description", "PROS",
// "Libs/UI/Textures/Dynamic/pros_reward.dds" -- member names are literal-pinned.

namespace wh::guimodule {

class C_UIFlashProsBanner : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashProsBanner;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1819A2598 (shared 4-string body)

    CryStringT<char> m_heading;       // +0x18  "ui_menu_prosAccount_heading"
    CryStringT<char> m_description;   // +0x20  "ui_pros_banner_Description"
    CryStringT<char> m_tag;           // +0x28  "PROS"
    CryStringT<char> m_texture;       // +0x30  "Libs/UI/Textures/Dynamic/pros_reward.dds"
    int32_t          m_unk38;         // +0x38  ctor stores *(int*)a6 (record int, @0x18239c7e1); not serialized by FillUIArgs [role UNVERIFIED]
    uint8_t          _pad3C[4];       // +0x3C
};
static_assert(sizeof(C_UIFlashProsBanner) == 0x40, "C_UIFlashProsBanner must be 0x40 (creator sub_181782924)");

}  // namespace wh::guimodule
