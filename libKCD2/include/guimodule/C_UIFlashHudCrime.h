#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashHudCrime -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_182B85FAC
// N=96 (ctor 0x182B61E7C, NOT decompiled this wave). FillUIArgs override 0x182B631A4.
// The +0x34..+0x4F tail almost certainly embeds a
// std::vector<shared_ptr<C_UIFlashHudCrimeStolenItem>> (sibling record) -- UNVERIFIED
// (flash_leaves.md §5.6), kept opaque until the ctor is traced.

namespace wh::guimodule {

class C_UIFlashHudCrime : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashHudCrime;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B631A4

    CryStringT<char> m_crimeName;   // +0x18
    CryStringT<char> m_icon;        // +0x20
    float            m_value;       // +0x28
    int32_t          m_int2C;       // +0x2C
    int32_t          m_int30;       // +0x30
    uint8_t          _pad34[4];     // +0x34  alignment padding (ctor never writes)
    std::vector<std::shared_ptr<C_UIFlashHudCrimeStolenItem>> m_stolenItems;  // +0x38  ctor push_back sub_182B5CFF4 (elem sz 16); dtor 0x182B62A54 frees [+0x38..+0x50)
};
static_assert(sizeof(C_UIFlashHudCrime) == 0x50, "C_UIFlashHudCrime must be 0x50 (creator sub_182B85FAC)");

}  // namespace wh::guimodule
