#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashMapPoiMarker -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1815B9998
// N=72 (72 - 0x10 _Ref_count_obj2 prefix = 0x38). vtable 0x183EE7858.
// FillUIArgs override 0x18186152C.
//
// TWO ctor overloads, one per POI SOURCE -- this is the split that matters:
//   0x1815B9C20  from a STATIC rpgmodule::I_POI&  (database POIs; the fast-travel points)
//   0x1815B9D70  from a DYNAMIC guimodule::S_EntityMapMark  (runtime entity marks)
//
// FIELD ORDER IS PROVEN against ActionScript, not guessed. FillUIArgs appends, in order:
//   base (C_UIFlashObject: m_id, m_str10) then
//   +0x18 string(tag 4, sub_1803C2034) | +0x20 int(sub_1803C1DBC) | +0x24 BOOL(tag 6,
//   sub_180555244) | +0x28 int(sub_1803C1DBC) | +0x2C float | +0x30 float
// PoiMarker.as SetData() reads dataA[start+0..6] as
//   0 UiName | 1 IconName | 2 (unused) | 3 m_IsFastTravel | 4 (unused) | 5 x | 6 y
// The x/y tail anchors the alignment, so index 3 <-> +0x24 is positional FACT.
//
// HARDCORE: the static ctor 0x1815B9C20 computes
//     m_isFastTravel = poi->IsFastTravel() && sub_181F4A2A0()
// so in wh::game::E_GameMode::Hardcore this bool is forced FALSE for every POI -- which is
// what makes flash refuse the double-click (DoAction.as gates onDoubleClicked on
// IsFastTravel). The marker is usually never even built in hardcore: the shared POI
// enumerator sub_18094EB9C drops type-7 POIs upstream. See E_GameMode.h for the full
// hardcore-gate family and C_PlayerData::m_flag2A for the second input to that gate.

namespace wh::guimodule {

class C_UIFlashMapPoiMarker : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashMapPoiMarker;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x18186152C

    CryStringT<char> m_name;     // +0x18  E_MarkType name (GetMarkTypeName sub_180C4D664) -> AS IconName
    int32_t          m_type;     // +0x20  marker-STATE param (= S_EntityMapMark::m_param, ctor sub_1815B9D70 copies src+8), NOT E_MarkType (that lives in m_name); shared domain with C_CompassMark::m_state (2 = discovered/shown observed). Static ctor forces 2 when m_isFastTravel.
    // AS-facing "this marker is a fast-travel destination" flag (PoiMarker.as
    // m_IsFastTravel, index 3). Serialized as a BOOL (tag 6) -- one byte, not an int.
    bool             m_isFastTravel;  // +0x24  static ctor: IsFastTravel() && sub_181F4A2A0()
    uint8_t          _pad25[3];  // +0x25
    int32_t          m_int28;    // +0x28  static ctor: I_POI [37] GetDefPriority (0x1815B9D4C)
    float            m_x;        // +0x2C  world position x
    float            m_y;        // +0x30  world position y
    uint8_t          _pad34[4];  // +0x34
};
static_assert(sizeof(C_UIFlashMapPoiMarker) == 0x38, "C_UIFlashMapPoiMarker must be 0x38 (creator sub_1815B9998)");

}  // namespace wh::guimodule
