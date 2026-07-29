#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListMapLegendUniqueItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1818994CC
// N=48 (ctor inlined). FillUIArgs override 0x1816829DC (shared base+int thunk).

namespace wh::guimodule {

class C_UIFlashListMapLegendUniqueItem : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListMapLegendUniqueItem;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1816829DC (shared base+int body)

    int32_t m_legendId;   // +0x18  raw tag-0 append [name descriptive]
    uint8_t _pad1C[4];    // +0x1C
};
static_assert(sizeof(C_UIFlashListMapLegendUniqueItem) == 0x20, "C_UIFlashListMapLegendUniqueItem must be 0x20 (creator sub_1818994CC)");

}  // namespace wh::guimodule
