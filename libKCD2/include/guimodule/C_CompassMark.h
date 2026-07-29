#pragma once
#include <cstdint>
#include <memory>
#include "guimodule/C_UIFlashObject.h"
#include "guimodule/E_MarkType.h"
#include "xgenaimodule/I_AreaTracker.h"
#include "xgenaimodule/I_ObjectManagerBaseListener.h"
#include "CryEngine/CryCommon/Cry_Math.h"   // Vec3

// -----------------------------------------------
// wh::guimodule::C_CompassMark -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_CompassMark@guimodule@wh@@ (TD 0x184C92818).
// Bases (CHD): I_AreaTracker @+0x00 (COL 0x413BAF8, vtable 0x183A6B4C8),
// I_ObjectManagerBaseListener @+0x08 (COL 0x413BB20, vtable 0x183A6B4B8),
// C_UIFlashObject @+0x10 (COL 0x413BB48, vtable 0x183A6B4F0),
// std::enable_shared_from_this<C_CompassMark> @+0x28.
// ctor sub_181F479C0(this, int type, C_LinkableObject* obj, const Vec3& pos, int param);
// dtor sub_180A19550 (deleting sub_180A19524).  ALWAYS shared_ptr-managed:
// make_shared sites sub_181F75F30 / in-place sub_181F443D0 (alloc 0x88 =
// _Ref_count_obj2<C_CompassMark> 0x10 + 0x78, TD 0x184C914B0).
//
// A LIVE compass/map marker.  THREE producers, all funnelling through sub_180C4D1E8
// (append to C_UICompass::m_marks @compass+0x60):
//   * LOCATION marks -- C_UICompass::Update refresh sub_1805673A8, from the RPG location
//     manager (S_GameContext+0x130), keyed by S_LocationId into m_marksByLocation @+0x78;
//   * POI marks -- C_UIMap::SetPoiMarkersVisible sub_182B02B34, one per visible
//     S_EntityMapMark (sub_181F47470 wrapper); C_UIMap::m_activeMarks @+0x5B8 maps the
//     S_EntityMapMark id -> shared_ptr<C_CompassMark>;
//   * QUEST-OBJECTIVE marks -- sub_180DC5F24 (per objective; driven by sub_180DC60B8 over a
//     quest's objectives, itself driven by sub_181F47200 over all active quests).  It alone
//     writes m_questColor(+0x68) and m_objectiveNumber(+0x6C) AFTER construction.
// Flash: discrete C_UICompass::AddCompassMarker sub_180C4D2E8 sends fc_addCompassMarker (all
// the fields tagged below); the per-frame C_UICompass tick sub_18065D1C4 re-sends the
// visible marks as the "CompassMarkers" array (FillUIArgs = a lean subset) + "UpdateCompass".
// The flash id (C_UIFlashObject::m_id, +0x18) = decimal string of ++dword_18548B09C -- fresh
// per mark, UNIQUE & monotonic (ctor sub_181F479C0).  When obj != null the ctor snaps m_pos
// from obj->vf[2]() (GetPos) and sets m_flag49 for object kinds 7/15 (byte obj+0x0F).
//
// [FUNDAMENTAL vs KCD1] KCD1 C_CompassMark (0x58) was {I_AreaTracker + raw fields,
// entityId + float pos + auto-inc int id}; KCD2 rebuilds it on C_UIFlashObject (string
// id + arg serialization), adds the object-removal listener base and
// enable_shared_from_this, and tracks a C_LinkableObject* instead of an entity id.

namespace wh::xgenaimodule { class C_LinkableObject; }

namespace wh::guimodule {

class C_CompassMark
    : public wh::xgenaimodule::I_AreaTracker                 // +0x00
    , public wh::xgenaimodule::I_ObjectManagerBaseListener   // +0x08
    , public C_UIFlashObject                                 // +0x10
    , public std::enable_shared_from_this<C_CompassMark>     // +0x28
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CompassMark;
    // I_AreaTracker: all four impls toggle m_playerInArea (see I_AreaTracker.h).
    void OnAreaEntered() override;   // 0x181A8C560
    void OnAreaLeft() override;      // 0x181A8C570
    void _vf2() override;            // 0x182B162FC
    void _vf3() override;            // 0x182B162FC (folded)
    // I_ObjectManagerBaseListener: event 1 (removed) -> detach (sub_180A195A4) + clear m_pTrackedObject.
    void OnObjectEvent(int event) override;   // 0x182B1438C
    // C_UIFlashObject: id/str + type + screen floats + in-area bytes -> SUIArguments.
    void FillUIArgs(void* pArgs) const override;   // 0x1805551D8

    C_UICompass* m_pOwnerCompass;  // +0x38  owning compass (AddMark 0x180C4D20F stores the C_UICompass* here); reused as the fc_addCompassMarker flash-dispatch target
    wh::xgenaimodule::C_LinkableObject* m_pTrackedObject;  // +0x40  ctor arg; nulled on object removal
    bool     m_flag48;         // +0x48  ctor 0 [role UNVERIFIED]
    bool     m_flag49;         // +0x49  ctor: 1 if tracked-object kind (obj+0x0F) is 7 or 15
    uint8_t  _pad4A[2];        // +0x4A
    E_MarkType::Type m_type;   // +0x4C  ctor arg `type`; POI marker-category (named by sub_180C4D664); fc_addCompassMarker `type`
    Vec3     m_pos;            // +0x50  ctor arg, overwritten from obj->vf[2]() when tracking an object
    int32_t  m_int5C;          // +0x5C  ctor 0 [role UNVERIFIED]
    int32_t  m_int60;          // +0x60  ctor 0; sent per-frame in the "CompassMarkers" array (FillUIArgs) [anglePitch/distance -- which is UNVERIFIED]
    float    m_distance;       // +0x64  distance to the marker; fc_addCompassMarker `distance` + FillUIArgs. ctor 0, refreshed per-frame
    int32_t  m_questColor;     // +0x68  quest color/tracking SLOT (sub_180DC5120 objName->slot index); fc_addCompassMarker `questColor`. ctor 0; set ONLY by quest producer sub_180DC5F24
    int32_t  m_objectiveNumber;// +0x6C  objective ordinal / A-B-C letter (sub_180DC61C8); fc_addCompassMarker `objectiveNumber`. ctor 0; set ONLY by quest producer sub_180DC5F24
    bool     m_playerInArea;   // +0x70  I_AreaTracker state; fc_addCompassMarker `isInsideArea` + FillUIArgs
    uint8_t  _pad71;           // +0x71
    bool     m_isInsideArea2D; // +0x72  ctor 0; fc_addCompassMarker `isInsideArea2D` (+ FillUIArgs `isLocked` slot)
    uint8_t  _pad73;           // +0x73
    int32_t  m_state;          // +0x74  ctor arg `param`; fc_addCompassMarker `state` (2 from map/quest/checkpoint paths); same domain as S_EntityMapMark::m_param and C_UIFlashMapPoiMarker+0x20 (candidate E_LocationMarkStates, not hard-typed)
};
static_assert(sizeof(C_CompassMark) == 0x78, "C_CompassMark must be 0x78");
static_assert(offsetof(C_CompassMark, m_pTrackedObject) == 0x40, "tracked object at 0x40");
static_assert(offsetof(C_CompassMark, m_pos) == 0x50, "position at 0x50");

}  // namespace wh::guimodule
