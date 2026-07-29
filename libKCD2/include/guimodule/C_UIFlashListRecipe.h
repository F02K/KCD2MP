#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListRecipe -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_18119EC10
// N=64 (ctor 0x18119EA30). FillUIArgs override 0x181753A84. Names descriptive
// (UNVERIFIED; the +0x20 gap is ctor-only).

namespace wh::guimodule {

class C_UIFlashListRecipe : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListRecipe;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181753A84

    CryStringT<char> m_name;     // +0x18
    int32_t          m_field20;   // +0x20  ctor writes 0/1/2/3 clamped from src getter (a2 vtbl+0x20); read+serialized in FillUIArgs 0x181753ABE. enum candidate {0,1,2,3} [role UNVERIFIED]
    bool             m_known;    // +0x24  inline tag-6 builder sub_181F44490
    uint8_t          _pad25[3];  // +0x25
    float            m_val28;    // +0x28  inline tag-1 builder
    int32_t          m_int2C;    // +0x2C  inline tag-0 builder sub_18157F350
};
static_assert(sizeof(C_UIFlashListRecipe) == 0x30, "C_UIFlashListRecipe must be 0x30 (creator sub_18119EC10)");

}  // namespace wh::guimodule
