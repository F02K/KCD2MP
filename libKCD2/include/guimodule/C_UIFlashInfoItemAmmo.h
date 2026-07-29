#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemAmmo -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78, 9 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_182B0C4D0 N=136 (ctor
// 0x182B6236C). FillUIArgs override 0x182B63410. Overrides [6] 0x182B6391C /
// [7] 0x18041A6A0; ADDS [8] 0x18055F540 (role UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoItemAmmo : public C_UIFlashInfoItem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemAmmo;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B63410
    void _vf6() override;                          // [6] 0x182B6391C
    bool _vf7() override;                          // [7] 0x18041A6A0
    virtual void _vf8();                           // [8] 0x18055F540  role UNVERIFIED

    float            m_ammoStats[6];   // +0x50..+0x68
    CryStringT<char> m_str68;          // +0x68
    CryStringT<char> m_str70;          // +0x70
};
static_assert(sizeof(C_UIFlashInfoItemAmmo) == 0x78, "C_UIFlashInfoItemAmmo must be 0x78 (creator sub_182B0C4D0)");

}  // namespace wh::guimodule
