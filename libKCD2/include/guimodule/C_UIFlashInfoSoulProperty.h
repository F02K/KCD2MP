#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoSoulProperty -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_180C42AE0
// N=96 (ctor 0x180C42954). FillUIArgs override 0x18183E7B0. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoSoulProperty : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoSoulProperty;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x18183E7B0

    CryStringT<char> m_name;     // +0x18
    int32_t          m_int20;    // +0x20
    uint8_t          _pad24[4];  // +0x24
    CryStringT<char> m_str28;    // +0x28
    int32_t          m_int30;    // +0x30
    int32_t          m_int34;    // +0x34
    int32_t          m_int38;    // +0x38
    float            m_val3C;    // +0x3C
    int32_t          m_int40;    // +0x40
    int32_t          m_int44;    // +0x44
    int32_t          m_int48;    // +0x48
    uint8_t          _pad4C[4];  // +0x4C
};
static_assert(sizeof(C_UIFlashInfoSoulProperty) == 0x50, "C_UIFlashInfoSoulProperty must be 0x50 (creator sub_180C42AE0)");

}  // namespace wh::guimodule
