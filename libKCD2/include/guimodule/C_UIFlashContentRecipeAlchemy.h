#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashContentRecipeAlchemy -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_182AFB510
// N=112 (ctor 0x182B61824; m_liquidBase literals "Spiritus"/"Oil"/"Wine"/"Water").
// FillUIArgs override 0x182B63038. m_ingredients element = stride-24
// {CryStringT, CryStringT, int32, int32}; m_vec48 element UNVERIFIED.

namespace wh::guimodule {

class C_UIFlashContentRecipeAlchemy : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashContentRecipeAlchemy;
    // Ingredient row (serialization-loop-proven stride 24).
    struct S_Ingredient {
        CryStringT<char> m_name;     // +0x00
        CryStringT<char> m_icon;     // +0x08
        int32_t          m_count;    // +0x10
        int32_t          m_int14;    // +0x14
    };

    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B63038

    CryStringT<char> m_name;               // +0x18
    CryStringT<char> m_str20;              // +0x20
    CryStringT<char> m_liquidBase;         // +0x28  "Spiritus"/"Oil"/"Wine"/"Water"
    std::vector<S_Ingredient> m_ingredients;  // +0x30  (0x18) stride 24
    std::vector<uint64_t> m_vec48;         // +0x48  (0x18) element type UNVERIFIED (flash_leaves.md §5.3)
};
static_assert(sizeof(C_UIFlashContentRecipeAlchemy) == 0x60, "C_UIFlashContentRecipeAlchemy must be 0x60 (creator sub_182AFB510)");
static_assert(sizeof(C_UIFlashContentRecipeAlchemy::S_Ingredient) == 24, "ingredient stride is 24");

}  // namespace wh::guimodule
