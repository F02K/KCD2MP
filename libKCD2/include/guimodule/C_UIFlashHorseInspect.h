#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashHorseInspect -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_182BB4FCC
// N=64 (ctor 0x182B61D30). FillUIArgs override 0x182B6314C. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashHorseInspect : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashHorseInspect;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B6314C

    CryStringT<char> m_name;     // +0x18
    int32_t          m_stat0;    // +0x20
    int32_t          m_stat1;    // +0x24
    int32_t          m_stat2;    // +0x28
    uint8_t          _pad2C[4];  // +0x2C
};
static_assert(sizeof(C_UIFlashHorseInspect) == 0x30, "C_UIFlashHorseInspect must be 0x30 (creator sub_182BB4FCC)");

}  // namespace wh::guimodule
