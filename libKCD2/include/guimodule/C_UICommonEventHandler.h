#pragma once
#include "Offsets/vtables/IUIElementEventListener.h"

// -----------------------------------------------
// wh::guimodule::C_UICommonEventHandler -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x8.
// -----------------------------------------------
// RTTI .?AVC_UICommonEventHandler@guimodule@wh@@ (TD 0x184CE2728).
// vtable 0x183C12408 (10 slots = IUIElementEventListener's 8 + 2 own).
// Stateless (vtable only): alloc 8 in C_GUIModule ctor sub_181789268 @0x18178936b,
// owned at C_GUIModule+0x28.
//
// Cross-cutting fscommand sink registered on EVERY flash element under the debug name
// "CommonEventHandler" (Init walks g_pFlashUI's element list and AddEventListener's
// itself; C_UIFlashBase's rebind path sub_180565FD0 re-attaches it per element).
// Slot [0] (sub_180557F3C) string-switches the event name:
//   "OnPlayAudio" / "OnStopAudio" / "OnSetVolumeAudio"  -> audio system calls
//     (object at *(gameGetter()+0xE8)+0x80; sub_180558548 / sub_182B018A8 / sub_182B0161C)
//   "GetButtonId"(name) -> input lookup via *(gameGetter()+0xE8)+0x30 vf[+0x18], and
//     WRITES TUIData{eInt, id} through the extra 5th listener argument (return-value
//     channel -- see the note in C_UIFlashBase.h).
//
// [FUNDAMENTAL vs KCD1] Replaces the KCD1 per-concern controller objects hanging off
// CUIManager (CUIInput/CUIGameEvents-style SUIEventReceiverDispatcher blocks) with one
// stateless listener shared by all elements.

namespace wh::guimodule {

class C_UICommonEventHandler : public Offsets::IUIElementEventListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UICommonEventHandler;
    // IUIElementEventListener [0..7]; [0] override = sub_180557F3C (banner), [7] = dtor
    // sub_182AFDA44. Own slots:
    virtual void Init();     // [8] 0x1805640FC  AddEventListener(this, "CommonEventHandler") on every IFlashUI element; called by module message-0 handler sub_180C3CA14
    virtual void Deinit();   // [9] 0x182BB8EB4  detach counterpart; called by C_GUIModule::Deinit sub_182B8C864
};
static_assert(sizeof(C_UICommonEventHandler) == 0x8, "C_UICommonEventHandler must be 0x8");

}  // namespace wh::guimodule
