#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItemDestructible.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemFood -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78, 8 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_182B0C62C N=136 (ctor
// 0x182B625C8). FillUIArgs override 0x182B63538 (re-serializes the Destructible layout
// + the food stat block).

namespace wh::guimodule {

class C_UIFlashInfoItemFood : public C_UIFlashInfoItemDestructible {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemFood;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B63538

    float            m_foodStats[5];   // +0x58..+0x6C
    uint8_t          _pad6C[4];        // +0x6C
    CryStringT<char> m_str70;          // +0x70
};
static_assert(sizeof(C_UIFlashInfoItemFood) == 0x78, "C_UIFlashInfoItemFood must be 0x78 (creator sub_182B0C62C)");

}  // namespace wh::guimodule
