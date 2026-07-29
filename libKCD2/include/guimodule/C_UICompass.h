#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>
#include "guimodule/C_UIFlashBase.h"
#include "guimodule/C_CompassMark.h"
#include "rpgmodule/I_LocationListener.h"
#include "rpgmodule/S_LocationId.h"         // = CryGUID (location.xml location_id)
#include "framework/HashPrimitives.h"       // wh::shared::S_DefaultHash
#include "CryEngine/CryCommon/Cry_Math.h"   // Vec3

// -----------------------------------------------
// wh::guimodule::C_UICompass -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x110.
// -----------------------------------------------
// RTTI .?AVC_UICompass@guimodule@wh@@ (TD 0x184CD0980).
// Bases: C_UIFlashBase @+0x00 (COL 0x4135C70), IUIElementEventListener @+0x10
// (COL 0x4135C98), rpgmodule::I_LocationListener @+0x58 (COL 0x4135CC0).
// vtables: primary 0x183C118B8 (14 slots), listener 0x183C11818 (8),
// I_LocationListener 0x183C11860 (10).  ctor sub_181786B0C; rttr-instantiated as a
// shared_ptr module screen (creator sub_1817869EC, alloc 0x110, adopt into
// std::_Ref_count<C_UICompass> TD 0x184CC67A0; constructor_wrapper as_std_shared_ptr
// TD 0x184CCE2C0) -- lives in C_GUIModule::m_uiElements, cached process-wide by the
// rttr element getter sub_180C4E098 (qword_18548B068).
//
// The HUD compass strip.  GetElementName [11] 0x181A719F0 = "HUD" (lives in the HUD
// flash movie, not a movie of its own).  KCD2-NEW as a class -- KCD1 compass traffic
// went through C_UIHUDElements.
//
// Overrides: [1] Init 0x180C3D0A8 (C_UIFlashBase::Init; registers the float CVar
// "wh_map_CompassPOIDistanceRatio" bound at +0xFC via IConsole vf+0x48; AddListener of
// the I_LocationListener subobject with the RPG location manager: S_GameContext+0x130
// -> +0xB8 -> vf+0x138); [2] Deinit 0x182B8CA40 (base + RemoveListener vf+0x140);
// [3] OnModuleMessage 0x18085DD54 (ids 72/76 -> global compass refresh sub_18197B198);
// [4] Update 0x18065D1C4 (drain; when the compass is enabled (module gate sub_180ED4C60
// vf[1](0)) and a player exists: tick apse camera member (owner apse via sub_1805677CC,
// +0x10C8 sub_181F49B40), refresh marks sub_1805673A8, collect visible m_marks'
// C_UIFlashObjects (visibility sub_18065CD84) and flash "CompassMarkers"(array) +
// "UpdateCompass"(heading from camera atan2f); [7]/[9] per-class rttr trio impls.
// I_LocationListener: [1] 0x181746AB0 location-state change -> S_LocationId (GUID) lookup
// in m_marksByLocation -> m_dirty; [2] 0x181A71940 m_dirty = 1 (FLIRT-mislabeled
// "decline_incoming_messages"); rest nullsub.
//
// C_UIMap pushes S_EntityMapMark-derived marks into m_marks via sub_180C4D1E8
// (append @+0x60 vector) and removes via sub_180A193C0.

namespace wh::guimodule {

class C_UICompass : public C_UIFlashBase, public wh::rpgmodule::I_LocationListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UICompass;
    std::vector<std::shared_ptr<C_CompassMark>> m_marks;   // +0x60  live compass marks
    // Double-buffered index of the live location marks, keyed by the location's S_LocationId
    // (= 128-bit GUID). Value = the C_CompassMark shown for that location. The refresh
    // (sub_1805673A8) swaps 0x78<->0xB8 each tick, drains the previous buffer, and rebuilds 0x78;
    // the I_LocationListener (0x181746AB0) looks a changed location up in m_marksByLocation -> m_dirty.
    // (K/V RESOLVED: node writes a 16-byte key @+0x10 then a 16-byte shared_ptr value @+0x20 whose
    //  raw ptr -> C_CompassMark +0x4C/+0x74 = m_type/m_state; both maps are the stock 0x40 _Hash.)
    std::unordered_map<wh::rpgmodule::S_LocationId, std::shared_ptr<C_CompassMark>,
                       wh::shared::S_DefaultHash<wh::rpgmodule::S_LocationId>> m_marksByLocation;      // +0x78  current frame
    std::unordered_map<wh::rpgmodule::S_LocationId, std::shared_ptr<C_CompassMark>,
                       wh::shared::S_DefaultHash<wh::rpgmodule::S_LocationId>> m_marksByLocationPrev;  // +0xB8  previous-frame buffer
    bool     m_dirty;                  // +0xF8  ctor 1; set by location-listener slots
    uint8_t  _padF9[3];                // +0xF9
    float    m_poiDistanceRatio;       // +0xFC  CVar-bound "wh_map_CompassPOIDistanceRatio"
    Vec3     m_pos100;                 // +0x100 ctor copy of globals qword_184932280/dword_184932288 [role UNVERIFIED]
    uint32_t _pad10C;                  // +0x10C
};
static_assert(sizeof(C_UICompass) == 0x110, "C_UICompass must be 0x110");
static_assert(offsetof(C_UICompass, m_marks) == 0x60, "marks vector at 0x60");
static_assert(offsetof(C_UICompass, m_marksByLocation) == 0x78, "location-mark map at 0x78");
static_assert(offsetof(C_UICompass, m_dirty) == 0xF8, "dirty flag at 0xF8");

}  // namespace wh::guimodule
