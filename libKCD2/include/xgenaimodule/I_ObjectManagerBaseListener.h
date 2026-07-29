#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_ObjectManagerBaseListener -- linkable-object lifecycle listener.
// KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_ObjectManagerBaseListener@xgenaimodule@wh@@ (TD 0x184C92740).  Survives from
// KCD1's T_ObjectManagerBase listener stack (wh::shared::C_Listeners<I_ObjectManagerBase-
// Listener,1> TD 0x184C9EC50 exists).  Single-slot vtable on both implementors analyzed:
//   guimodule::C_CompassMark   @+0x08 (vtable 0x183A6B4B8): [0] 0x182B1438C
//     -> if (event == 1) detach from tracked object (sub_180A195A4) + clear m_pTrackedObject
//   guimodule::C_ShowMapMarker @+0x88 (vtable 0x183B72D88): [0] 0x182B37FEC
//     -> if (event == 1) remove the map mark (untrigger sub_180A19B80)
// Registration: keyed by the object's WUID (linkable+0x08) via sub_180C9B198 /
// unregister sub_180A198C0 on the object manager at S_GameContext+0x160 -> +0xA8 -> vf+0xF0.
//
// event 1 = object removed (KCD1 E_AIONotifyEvent::Removed); other codes unobserved here.

namespace wh::xgenaimodule {

class I_ObjectManagerBaseListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ObjectManagerBaseListener;
    virtual void OnObjectEvent(int event) = 0;   // [0] event 1 = object removed
};
static_assert(sizeof(I_ObjectManagerBaseListener) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::xgenaimodule
