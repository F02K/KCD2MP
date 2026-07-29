#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashHudCrimeStolenItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_182B5F72C
// N=56 (ctor 0x182B62034). FillUIArgs override 0x182B6320C. Sibling record of
// C_UIFlashHudCrime (likely its stolen-item list element). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashHudCrimeStolenItem : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashHudCrimeStolenItem;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B6320C

    CryStringT<char> m_name;     // +0x18
    int32_t          _unk20;     // +0x20  ctor copy 0x182B62102 (src+0); flash int arg, same builder as m_count
    int32_t          m_count;    // +0x24  inline tag-0 builder sub_18157F350
};
static_assert(sizeof(C_UIFlashHudCrimeStolenItem) == 0x28, "C_UIFlashHudCrimeStolenItem must be 0x28 (creator sub_182B5F72C)");

}  // namespace wh::guimodule
