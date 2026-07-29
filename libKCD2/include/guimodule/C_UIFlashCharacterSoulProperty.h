#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashCharacterSoulProperty -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_18055A448
// N=80 (ctor 0x18055ABC8). FillUIArgs override 0x181620170. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashCharacterSoulProperty : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashCharacterSoulProperty;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x181620170

    CryStringT<char> m_name;     // +0x18
    CryStringT<char> m_icon;     // +0x20
    int32_t          m_val0;     // +0x28
    int32_t          m_val1;     // +0x2C
    int32_t          m_val2;     // +0x30
    int32_t          m_val3;     // +0x34
    bool             m_flag;     // +0x38
    uint8_t          _pad39[7];  // +0x39
};
static_assert(sizeof(C_UIFlashCharacterSoulProperty) == 0x40, "C_UIFlashCharacterSoulProperty must be 0x40 (creator sub_18055A448)");

}  // namespace wh::guimodule
