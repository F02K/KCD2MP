#pragma once
#include <cstdint>
#include <map>
#include "playermodule/I_FogOfWarListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIMapCloudAtlas -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI .?AVC_UIMapCloudAtlas@guimodule@wh@@ (TD 0x184C92A00, COL 0x413BEE0).
// Base: playermodule::I_FogOfWarListener @+0x00 (CHD).  vtable 0x183B8EED0 (4 slots =
// the listener interface; dtor 0x182AFD97C resets the vtable to
// I_FogOfWarListener::`vftable' -- base identity binary-confirmed).
// ctor sub_18146C69C(this, C_UIMap* owner) -- embedded BY VALUE in C_UIMap at +0x698.
//
// Per-level map "cloud" (undiscovered-area overlay) registry, keyed by level name.
// Reset/reload sub_180D0DC34: clears the map, then parses "Libs\UI\mapClouds.xml"
// (ISystem::LoadXmlFromFile, qword_18492D8C0 vf+0x418), one S_Cloud (alloc 0xD8, ctor
// sub_180D0DE24(cloud, m_pUIMap)) per <level Name=...> node.  Called from the C_UIMap
// ctor tail... (empty-clear), the save-load reset (sub_180DCA18C) and module message
// 0x18.  Listener slots forward reveal events into the named level's S_Cloud
// (see I_FogOfWarListener.h).
//
// [MODERATE vs KCD1] KCD1 was 0x18 {vtable, std::map<const char*, void*>}; KCD2 adds
// the owner backref at +0x08 and the values are the RE-identified S_Cloud objects.

namespace wh::guimodule {

class C_UIMap;
// Per-level cloud grid (RTTI .?AUS_Cloud@guimodule@wh@@, TD 0x184C92A38; alloc 0xD8,
// ctor sub_180D0DE24, XML parse sub_180D0E0B0, release sub_180D0EB90).  Not modeled
// this wave -- pointer use only.
struct S_Cloud;

class C_UIMapCloudAtlas : public wh::playermodule::I_FogOfWarListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIMapCloudAtlas;
    ~C_UIMapCloudAtlas() override;                                                          // [0] 0x182AFD97C
    void OnFogOfWarChanged(int a2, const CryStringT<char>& levelName, int a4, int a5) override;  // [1] 0x180B57E5C
    void OnCurrentLevelFogChanged(void* a2, void* a3) override;                             // [2] 0x182AFFC60
    void OnLevelFogReset(void* a2, const CryStringT<char>& levelName) override;             // [3] 0x180C3D6E4

    C_UIMap* m_pUIMap;                              // +0x08  owner backref (ctor a2)
    std::map<CryStringT<char>, S_Cloud*> m_clouds;  // +0x10  level name -> owned S_Cloud (clear sub_180D0DA3C deletes values)
};
static_assert(sizeof(C_UIMapCloudAtlas) == 0x20, "C_UIMapCloudAtlas must be 0x20");

}  // namespace wh::guimodule
