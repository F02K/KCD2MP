#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItemWeapon.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemMelee -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD0, 9 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_18055F630 N=224 (ctor
// 0x18055F1D8). FillUIArgs override 0x180556DB8. Overrides [6] 0x18055F714 /
// [7] 0x18041A6A0; ADDS [8] 0x18055F540 (role UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoItemMelee : public C_UIFlashInfoItemWeapon {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemMelee;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x180556DB8
    void _vf6() override;                          // [6] 0x18055F714
    bool _vf7() override;                          // [7] 0x18041A6A0
    virtual void _vf8();                           // [8] 0x18055F540  role UNVERIFIED

    float            m_meleeStats[15];   // +0x78..+0xB4  contiguous 4B floats [per-slot roles UNVERIFIED]
    uint8_t          _padB4[4];          // +0xB4
    CryStringT<char> m_strB8;            // +0xB8
    CryStringT<char> m_strC0;            // +0xC0
    CryStringT<char> m_strC8;            // +0xC8
};
static_assert(sizeof(C_UIFlashInfoItemMelee) == 0xD0, "C_UIFlashInfoItemMelee must be 0xD0 (creator sub_18055F630)");

}  // namespace wh::guimodule
