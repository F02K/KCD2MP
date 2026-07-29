#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemEquippable -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x68, 8 slots.
// -----------------------------------------------
// INTERMEDIATE InfoItem-subtree record (flash_leaves.md §3.3/§5.5): NO make_shared
// creator -- instantiated only through C_UIFlashInfoItemArmor / C_UIFlashInfoItemWeapon.
// Binary member extent is 0x64 (FillUIArgs sub_1815794E0, last int @+0x60); C++ sizeof
// pads to 0x68 (align 8) -- the derived classes' members start at +0x68, matching their
// alloc-proven layouts (Armor floats @+0x68, Weapon ints @+0x68). The Armor dossier's
// "+0x64 gap" is this tail padding. Overrides [6] 0x18055F8A8 / [7] 0x18041A6A0.

namespace wh::guimodule {

class C_UIFlashInfoItemEquippable : public C_UIFlashInfoItem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemEquippable;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1815794E0
    void _vf6() override;                          // [6] 0x18055F8A8
    bool _vf7() override;                          // [7] 0x18041A6A0

    float   m_f50;     // +0x50
    int32_t m_int54;   // +0x54  tag-2 sub_1803C1F4C
    int32_t m_int58;   // +0x58
    int32_t m_int5C;   // +0x5C
    int32_t m_int60;   // +0x60  (+0x64 = tail padding)
};
static_assert(sizeof(C_UIFlashInfoItemEquippable) == 0x68, "C_UIFlashInfoItemEquippable is 0x68 (0x64 extent + align; intermediate, not alloc-proven)");

}  // namespace wh::guimodule
