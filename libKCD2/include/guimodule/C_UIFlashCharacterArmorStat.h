#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "guimodule/C_UIFlashCharacterArmorStatSub.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCharacterArmorStat -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_180C4B198
// N=64 (ctor 0x1808F4E38). FillUIArgs [1] = base thunk 0x181851FE8 (no own scalars);
// the child vector is serialized by the _vf2 [2] override 0x180557018 (count, then each
// element's FillUIArgs/_vf2; loop stride 16 = shared_ptr).

namespace wh::guimodule {

class C_UIFlashCharacterArmorStat : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCharacterArmorStat;
    void _vf2(void* pArgs) const override;   // [2] 0x180557018  serializes m_subs

    std::vector<std::shared_ptr<C_UIFlashCharacterArmorStatSub>> m_subs;   // +0x18  (0x18)
};
static_assert(sizeof(C_UIFlashCharacterArmorStat) == 0x30, "C_UIFlashCharacterArmorStat must be 0x30 (creator sub_180C4B198)");

}  // namespace wh::guimodule
