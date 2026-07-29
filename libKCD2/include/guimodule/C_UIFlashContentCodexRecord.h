#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashContentCodexRecord -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1808BEFB4
// N=64 (ctor 0x1808BF07C). FillUIArgs override 0x181764264. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashContentCodexRecord : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashContentCodexRecord;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181764264

    CryStringT<char> m_title;    // +0x18
    CryStringT<char> m_body;     // +0x20
    int32_t          m_recId;    // +0x28  raw tag-0 append of *(this+0x28)
    uint8_t          _pad2C[4];  // +0x2C
};
static_assert(sizeof(C_UIFlashContentCodexRecord) == 0x30, "C_UIFlashContentCodexRecord must be 0x30 (creator sub_1808BEFB4)");

}  // namespace wh::guimodule
