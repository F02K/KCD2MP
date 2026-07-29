#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItemWeapon.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemMissile -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88, 9 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_182B0C6DC N=152 (ctor
// 0x182B62734). FillUIArgs override 0x182B635B4. Overrides [6] 0x182B63974 /
// [7] 0x18041A6A0; ADDS [8] 0x18055F540 (role UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoItemMissile : public C_UIFlashInfoItemWeapon {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemMissile;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B635B4
    void _vf6() override;                          // [6] 0x182B63974
    bool _vf7() override;                          // [7] 0x18041A6A0
    virtual void _vf8();                           // [8] 0x18055F540  role UNVERIFIED

    float            m_f78;       // +0x78  ctor 0x182B62788 = S_MissileWeaponItemClass+0x138 (float def 1.0f); FillUIArgs tag-1 numeric variant
    float            m_f7C;       // +0x7C  inline tag-1 builder
    CryStringT<char> m_str80;     // +0x80
};
static_assert(sizeof(C_UIFlashInfoItemMissile) == 0x88, "C_UIFlashInfoItemMissile must be 0x88 (creator sub_182B0C6DC)");

}  // namespace wh::guimodule
