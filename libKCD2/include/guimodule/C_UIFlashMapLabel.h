#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashMapLabel -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_180D118A4
// N=56 (ctor 0x180D117E0). FillUIArgs override 0x1819DD798. Feeds the map's label layer
// (S_MapLabel/C_MapLabelDatabase is the data source). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashMapLabel : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashMapLabel;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1819DD798

    float   m_x;       // +0x18
    float   m_y;       // +0x1C
    int32_t m_int20;   // +0x20
    int32_t m_int24;   // +0x24
};
static_assert(sizeof(C_UIFlashMapLabel) == 0x28, "C_UIFlashMapLabel must be 0x28 (creator sub_180D118A4)");

}  // namespace wh::guimodule
