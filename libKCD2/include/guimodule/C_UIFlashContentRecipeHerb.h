#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashContentRecipeHerb -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_182AFB5BC
// N=72 (ctor 0x182B61A3C). FillUIArgs override 0x1819A2598 (COMDAT-shared with
// C_UIFlashProsBanner -- 4-string serializer). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashContentRecipeHerb : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashContentRecipeHerb;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1819A2598 (shared 4-string body)

    CryStringT<char> m_str0;   // +0x18
    CryStringT<char> m_str1;   // +0x20
    CryStringT<char> m_str2;   // +0x28
    CryStringT<char> m_str3;   // +0x30
};
static_assert(sizeof(C_UIFlashContentRecipeHerb) == 0x38, "C_UIFlashContentRecipeHerb must be 0x38 (creator sub_182AFB5BC)");

}  // namespace wh::guimodule
