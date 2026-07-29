#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListMapLegendItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1815B9A60
// N=80 (80 - 0x10 _Ref_count_obj2 prefix = 0x40; ctor 0x1815B9B38). vtable 0x183BC4300
// (6 slots; ??_R4 of the next object sits at 0x183BC4330). FillUIArgs override
// 0x1818AA90C appends +0x18 int (sub_1803C1D4C), +0x1C int (sub_1803C1DBC), +0x20 BOOL
// (tag 6, sub_180555244) -- the m_items vector is NOT serialized. Names descriptive
// (UNVERIFIED); the +0x21 tail is ctor-only.
//
// One legend/filter row of the map legend list. Built per emitted POI by the map's POI
// callback sub_181F48460; that callback special-cases E_MarkType::FastTravel (7) and skips
// the legend row entirely when sub_181F4A2A0() is false (i.e. in
// wh::game::E_GameMode::Hardcore), so hardcore loses the fast-travel legend entry as well
// as the markers. Mark types 8 and 96 take their own branches and are NOT gated there.
//
// The m_items tail fills only when the source POI's GetDefMarkType() == 48 (GeneralPoi):
// the ctor then runs the shared POI enumerator sub_18094EB9C with its own lambda
// (vtable 0x183BC4300-adjacent _Func_impl_no_alloc<...,void,I_POI&>) to gather the
// sub-entries. That is the third of the enumerator's three call sites.

namespace wh::guimodule {

class C_UIFlashListMapLegendUniqueItem;   // RTTI .?AVC_UIFlashListMapLegendUniqueItem@guimodule@wh@@

class C_UIFlashListMapLegendItem : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListMapLegendItem;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1818AA90C

    int32_t m_type;         // +0x18
    int32_t m_int1C;        // +0x1C
    bool    m_bool20;       // +0x20
    uint8_t _pad21[7];      // +0x21  alignment padding (ctor never writes +0x21..+0x27)
    std::vector<std::shared_ptr<C_UIFlashListMapLegendUniqueItem>> m_items;   // +0x28  (0x18) unique legend entries; shared_ptr built per POI by sub_1818994CC, emplaced when mark type==48
};
static_assert(sizeof(C_UIFlashListMapLegendItem) == 0x40, "C_UIFlashListMapLegendItem must be 0x40 (creator sub_1815B9A60)");

}  // namespace wh::guimodule
