#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItemWeapon.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemOffHand -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88, 9 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_182B0C78C N=152 (ctor
// 0x182B6285C). FillUIArgs override 0x182B6369C. Overrides [6] 0x182B639B8 /
// [7] 0x18041A6A0; ADDS [8] 0x18055F540 (role UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoItemOffHand : public C_UIFlashInfoItemWeapon {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemOffHand;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B6369C
    void _vf6() override;                          // [6] 0x182B639B8
    bool _vf7() override;                          // [7] 0x18041A6A0
    virtual void _vf8();                           // [8] 0x18055F540  role UNVERIFIED

    float   m_f78;      // +0x78
    float   m_f7C;      // +0x7C
    float   m_f80;      // +0x80
    bool    m_bool84;   // +0x84
    uint8_t _pad85[3];  // +0x85
};
static_assert(sizeof(C_UIFlashInfoItemOffHand) == 0x88, "C_UIFlashInfoItemOffHand must be 0x88 (creator sub_182B0C78C)");

}  // namespace wh::guimodule
