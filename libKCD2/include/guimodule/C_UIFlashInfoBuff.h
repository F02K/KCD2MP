#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoBuff -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_182AFB658
// N=72 (ctor 0x182B62128). FillUIArgs override 0x182B632F8. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoBuff : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoBuff;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B632F8

    CryStringT<char> m_name;     // +0x18
    int32_t          m_int20;    // +0x20
    float            m_value;    // +0x24
    int32_t          m_int28;    // +0x28
    uint8_t          _pad2C[4];  // +0x2C
    CryStringT<char> m_icon;     // +0x30
};
static_assert(sizeof(C_UIFlashInfoBuff) == 0x38, "C_UIFlashInfoBuff must be 0x38 (creator sub_182AFB658)");

}  // namespace wh::guimodule
