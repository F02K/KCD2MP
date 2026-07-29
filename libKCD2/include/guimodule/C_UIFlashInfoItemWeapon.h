#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItemEquippable.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemWeapon -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78, 8 slots.
// -----------------------------------------------
// INTERMEDIATE InfoItem-subtree record (flash_leaves.md §3.3/§5.5): NO make_shared
// creator -- instantiated only through Melee/Missile/OffHand. Extent 0x78 from
// FillUIArgs sub_181579490 (last int @+0x74); intermediate, not alloc-proven.
// Overrides [6] 0x18055F8A8 / [7] 0x18041A6A0 (same bodies as Equippable).

namespace wh::guimodule {

class C_UIFlashInfoItemWeapon : public C_UIFlashInfoItemEquippable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemWeapon;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181579490

    int32_t m_int68;   // +0x68
    int32_t m_int6C;   // +0x6C
    int32_t m_int70;   // +0x70
    int32_t m_int74;   // +0x74
};
static_assert(sizeof(C_UIFlashInfoItemWeapon) == 0x78, "C_UIFlashInfoItemWeapon must be 0x78 (extent; intermediate, not alloc-proven)");

}  // namespace wh::guimodule
