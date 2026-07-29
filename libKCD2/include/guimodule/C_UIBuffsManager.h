#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIBase.h"
#include "framework/C_Listeners.h"

// -----------------------------------------------
// wh::guimodule::C_UIBuffsManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x68.
// -----------------------------------------------
// RTTI .?AVC_UIBuffsManager@guimodule@wh@@ (TD 0x184CE2768, COL 0x41344D8).
// vtable 0x183C12498 (10 slots -- pure C_UIBase shape, no additions).
// ctor sub_1817894B8 (alloc 0x68) in the C_GUIModule ctor, owned at C_GUIModule+0x38
// (NOT in the screen vector -- the module broadcasts vf[1]/vf[2]/vf[3]/vf[4] to it
// explicitly alongside the element loop).
//
// The player-buff HUD FEED: it is a C_UIBase but not flash-bound; Init (slot [1],
// sub_180C3CA80) registers a {this, sub_18047B3C4} delegate with the buff system
// (sub_180E820D4() vf[+0x58] add / Deinit sub_182BB8E78 vf[+0x60] remove). Update
// (slot [4], sub_18046D478) pulls the player soul's live buff list (soul vf[+0x370]),
// filters HUD-visible entries (sub_18046D688), diffs against m_displayedBuffs and
// NOTIFIES m_listeners of adds/removes (broadcast helper sub_18170BCD8 -> C_Listeners
// dispatch sub_181F94940); the flash HUD leaf subscribes as an I_UIBuffsListener.
// OnModuleMessage (slot [3], sub_18085DCF8) handles resets.
//
// [FUNDAMENTAL vs KCD1] KCD1 buff HUD state lived in the C_UIRPGElements event system;
// KCD2 splits data (this manager + listener interface) from presentation (flash leaf).

namespace wh::guimodule {

class I_UIBuffsListener;   // notification sink (RTTI .?AVI_UIBuffsListener@guimodule@wh@@)

class C_UIBuffsManager : public C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIBuffsManager;
    wh::shared::C_Listeners<I_UIBuffsListener, 1> m_listeners;  // +0x10  EMBEDDED (ctor installs C_Listeners<I_UIBuffsListener,1>::vftable @+0x10; cursor init sub_180A03994(+0x30, 1, -1))
    std::vector<uint64_t> m_displayedBuffs;   // +0x38  buff ids currently on the HUD (stride-8 diff loops in Update)
    uint64_t m_50;                            // +0x50  ctor 0 (role UNVERIFIED)
    uint64_t m_58;                            // +0x58  ctor 0 (role UNVERIFIED)
    uint64_t m_60;                            // +0x60  ctor 0 (role UNVERIFIED)
};
static_assert(sizeof(C_UIBuffsManager) == 0x68, "C_UIBuffsManager must be 0x68");

}  // namespace wh::guimodule
