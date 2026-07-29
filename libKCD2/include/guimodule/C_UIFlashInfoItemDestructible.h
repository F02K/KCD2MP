#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemDestructible -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x58, 8 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_182B0C580 N=104 (ctor
// 0x182B62564). FillUIArgs override 0x182B6349C. Inherits [6] 0x18055F8F4 /
// [7] 0x180838AE0 (base impls).

namespace wh::guimodule {

class C_UIFlashInfoItemDestructible : public C_UIFlashInfoItem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemDestructible;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B6349C

    float   m_float50;   // +0x50  ctor movss [rsi+50h]=sub_18096F7D4(src) (float); FillUIArgs reads as GFxValue number
    int32_t m_int54;     // +0x54  tag-2 sub_1803C1F4C
};
static_assert(sizeof(C_UIFlashInfoItemDestructible) == 0x58, "C_UIFlashInfoItemDestructible must be 0x58 (creator sub_182B0C580)");

}  // namespace wh::guimodule
