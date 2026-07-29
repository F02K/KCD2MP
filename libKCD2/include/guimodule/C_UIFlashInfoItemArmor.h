#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItemEquippable.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemArmor -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD8, 9 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_181494CB0 N=232 (ctor
// 0x181494D8C). FillUIArgs override 0x181579300 (re-serializes the InfoItem base +
// Equippable ints + the armor-stat float block; exact per-float offsets in the G4b
// scratch flash_members.txt). Overrides [6] 0x181494B80 / [7] 0x18041A6A0; ADDS
// [8] 0x18149507C (role UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoItemArmor : public C_UIFlashInfoItemEquippable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemArmor;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181579300
    void _vf6() override;                          // [6] 0x181494B80
    bool _vf7() override;                          // [7] 0x18041A6A0
    virtual void _vf8();                           // [8] 0x18149507C  role UNVERIFIED

    float            m_armorStats[23];   // +0x68..+0xC4  contiguous 4B floats [per-slot roles UNVERIFIED]
    uint8_t          _padC4[4];          // +0xC4
    CryStringT<char> m_strC8;            // +0xC8
    CryStringT<char> m_strD0;            // +0xD0
};
static_assert(sizeof(C_UIFlashInfoItemArmor) == 0xD8, "C_UIFlashInfoItemArmor must be 0xD8 (creator sub_181494CB0)");
static_assert(offsetof(C_UIFlashInfoItemArmor, m_armorStats) == 0x68, "armor stat block at 0x68");

}  // namespace wh::guimodule
