#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "guimodule/C_UIFlashCharacterWeaponStatSub.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCharacterWeaponStat -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_1808F5948
// N=64 (ctor 0x1808F5A24). FillUIArgs [1] = base thunk 0x181851FE8; child vector
// serialized by the _vf2 [2] override 0x180557018.

namespace wh::guimodule {

class C_UIFlashCharacterWeaponStat : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCharacterWeaponStat;
    void _vf2(void* pArgs) const override;   // [2] 0x180557018  serializes m_subs

    std::vector<std::shared_ptr<C_UIFlashCharacterWeaponStatSub>> m_subs;   // +0x18  (0x18)
};
static_assert(sizeof(C_UIFlashCharacterWeaponStat) == 0x30, "C_UIFlashCharacterWeaponStat must be 0x30 (creator sub_1808F5948)");

}  // namespace wh::guimodule
