#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemDocument -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x58, 8 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_18119ECF8 N=104 (ctor
// 0x18119EDD8). FillUIArgs override 0x181579284. Inherits [6]/[7] base impls.

namespace wh::guimodule {

class C_UIFlashInfoItemDocument : public C_UIFlashInfoItem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemDocument;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181579284

    bool    m_readable;   // +0x50  raw tag-6 (inline builder)
    uint8_t _pad51[7];    // +0x51
};
static_assert(sizeof(C_UIFlashInfoItemDocument) == 0x58, "C_UIFlashInfoItemDocument must be 0x58 (creator sub_18119ECF8)");

}  // namespace wh::guimodule
