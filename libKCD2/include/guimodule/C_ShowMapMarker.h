#pragma once
#include <cstdint>
#include <memory>
#include "conceptmodule/C_Effect.h"
#include "conceptmodule/C_TypedPortRef.h"
#include "guimodule/E_ConceptMarkerType.h"
#include "guimodule/S_EntityMapMark.h"
#include "xgenaimodule/I_ObjectManagerBaseListener.h"

// -----------------------------------------------
// wh::guimodule::C_ShowMapMarker -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x120.
// -----------------------------------------------
// RTTI .?AVC_ShowMapMarker@guimodule@wh@@ (TD 0x184CB0A58).
// Bases (CHD): conceptmodule::C_Effect @+0x00 (chain C_SharedResource -> C_Node ->
// C_AutoTriggerable<C_Node> -> C_Effect; COL 0x4139708, vtable 0x183B72E58, 46 slots),
// xgenaimodule::I_ObjectManagerBaseListener @+0x88 (COL 0x4139730, vtable 0x183B72D88).
// ctor sub_1813AFF18; creators: as_wh_smart_ptr sub_1813AFE60 (alloc 0x120,
// C_SharedResource addref/release sub_181E38F20/sub_181E39320) and as_std_shared_ptr
// sub_182B2DAF0 (_Ref_count<C_ShowMapMarker> TD 0x184CA8750).
//
// rttr class "wh::guimodule::ShowMapMarker" (registration sub_1812409C8) with
// member-object properties "MarkerObject" (m_markerObjectPort) and "MarkerType"
// (m_markerTypePort) -- a concept-graph (conceptmodule) trigger/effect node that, while
// triggered, shows a map/compass marker on a target object.  KCD2-NEW.
//
// Vtable: inherits the 46-slot conceptmodule::C_Effect table (per module convention the
// concept-node slot map is not modeled as C++ virtuals yet); own slots by DIFF vs the
// C_Effect vtable 0x183A45F30:
//   [0]  0x1815280E4  deleting dtor
//   [5]/[7]           per-class rttr get_type/get_derived_info (trio at [5..7])
//   [27] 0x182B37A70  wrap self as rttr variant {_smart_ptr<C_ShowMapMarker>,
//        "ShowMapMarker", "wh::guimodule", type id 13} [slot role: describe/box self]
//   [43] 0x180C4DADC  TRIGGER (pure in C_Effect): resolve MarkerObject port
//        (sub_1813C0B1C) -> C_UIMap (module -> apse sub_1805677CC -> +0x1508) ->
//        GetOrCreateEntityMark(sub_180C4DBD8; type = MarkerType port converted by
//        sub_180C4DC78) -> store in m_pMark -> RegisterEntityMark (sub_180C4DD5C)
//        -> add self+0x88 as object-removal listener (sub_180C9B198, keyed by the
//        target's WUID @linkable+0x08)
//   [44] 0x180A19B78 -> 0x180A19B80  UNTRIGGER (pure in C_Effect): unregister the
//        removal listener (sub_180A198C0), C_UIMap::RemoveEntityMark (sub_180A182F0),
//        reset m_pMark
// I_ObjectManagerBaseListener [0] 0x182B37FEC: event 1 (object removed) -> untrigger.

namespace wh::xgenaimodule { class C_LinkableObject; }

namespace wh::guimodule {

class C_ShowMapMarker
    : public wh::conceptmodule::C_Effect                     // +0x00 (0x88)
    , public wh::xgenaimodule::I_ObjectManagerBaseListener   // +0x88
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ShowMapMarker;
    // Original source carries RTTR_ENABLE(C_Effect): registered
    // "wh::guimodule::C_ShowMapMarker" (get_type 0x1813693D8, sizeof imm 0x120), base
    // list lambda 0x180E23CC0 appends C_Effect (sole direct base; C_Effect itself HAS
    // further bases, lambda 0x180C395A0). Trio = primary-vtable overrides at [5..7].
    // NOT declared here because the conceptmodule tranche does not model the 46-slot
    // C_Effect vtable as C++ virtuals yet -- adding only the trio would mis-place it
    // at slots [0..2]. Migrate together with the conceptmodule virtual-slot wave.
    // I_ObjectManagerBaseListener: target removed -> hide the marker.
    void OnObjectEvent(int event) override;   // 0x182B37FEC

    // Concept-graph input ports (rttr properties "MarkerObject" / "MarkerType").
    wh::conceptmodule::C_TypedPortRef<wh::xgenaimodule::C_LinkableObject*> m_markerObjectPort;  // +0x90  (0x40)
    wh::conceptmodule::C_TypedPortRef<E_ConceptMarkerType::Type>           m_markerTypePort;    // +0xD0  (0x40)
    // The persistent mark record created on trigger (null while untriggered).
    std::shared_ptr<S_EntityMapMark> m_pMark;   // +0x110
};
static_assert(sizeof(C_ShowMapMarker) == 0x120, "C_ShowMapMarker must be 0x120");
static_assert(offsetof(C_ShowMapMarker, m_markerObjectPort) == 0x90, "MarkerObject port at 0x90");
static_assert(offsetof(C_ShowMapMarker, m_markerTypePort) == 0xD0, "MarkerType port at 0xD0");
static_assert(offsetof(C_ShowMapMarker, m_pMark) == 0x110, "mark shared_ptr at 0x110");

}  // namespace wh::guimodule
