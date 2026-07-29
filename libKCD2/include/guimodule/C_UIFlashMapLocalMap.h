#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashMapLocalMap -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x48 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_181932710
// N=88 (ctor 0x1819327D8). FillUIArgs override 0x1819BD988. Mirrors the S_LocalMap
// table data (position/active-area/tiles). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashMapLocalMap : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashMapLocalMap;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1819BD988

    float   m_f18;      // +0x18
    float   m_f1C;      // +0x1C
    float   m_f20;      // +0x20
    float   m_f24;      // +0x24
    float   m_f28;      // +0x28
    float   m_f2C;      // +0x2C
    float   m_f30;      // +0x30
    float   m_f34;      // +0x34
    bool    m_bool38;   // +0x38
    uint8_t _pad39[3];  // +0x39
    int32_t m_int3C;    // +0x3C
    int32_t m_int40;    // +0x40
    int32_t m_int44;    // +0x44
};
static_assert(sizeof(C_UIFlashMapLocalMap) == 0x48, "C_UIFlashMapLocalMap must be 0x48 (creator sub_181932710)");

}  // namespace wh::guimodule
