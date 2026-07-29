#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListBuff -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_18146BD38
// N=64 (ctor 0x18146BE10). FillUIArgs override 0x18171CE40. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashListBuff : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListBuff;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x18171CE40

    CryStringT<char> m_icon;    // +0x18
    int32_t          m_int20;   // +0x20
    int32_t          m_int24;   // +0x24
    float            m_value;   // +0x28
    int32_t          m_int2C;   // +0x2C
};
static_assert(sizeof(C_UIFlashListBuff) == 0x30, "C_UIFlashListBuff must be 0x30 (creator sub_18146BD38)");

}  // namespace wh::guimodule
