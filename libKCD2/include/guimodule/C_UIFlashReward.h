#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashReward -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_182BA43BC
// N=56 (ctor 0x182B8AB88). FillUIArgs override 0x182B8D4A4 (two strings only).

namespace wh::guimodule {

class C_UIFlashReward : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashReward;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B8D4A4

    CryStringT<char> m_title;   // +0x18
    CryStringT<char> m_icon;    // +0x20
};
static_assert(sizeof(C_UIFlashReward) == 0x28, "C_UIFlashReward must be 0x28 (creator sub_182BA43BC)");

}  // namespace wh::guimodule
