#pragma once
#include <cstdint>
#include "playermodule/I_UIActionHintManager.h"
#include "Offsets/vtables/IGame.h"   // Offsets::IInputEventListener (4 slots, no virtual dtor)
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_UIActionHintManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38.
// -----------------------------------------------
// RTTI .?AVC_UIActionHintManager@guimodule@wh@@ (TD 0x184CE2800).
// Bases: wh::playermodule::I_UIActionHintManager @+0x00 (COL 0x4134488, vtable
// 0x183C12518, 6 slots), IInputEventListener @+0x08 (COL 0x41344B0, vtable 0x183C124F0,
// 4 slots). ctor = factory sub_181789544 (alloc 0x38) in the C_GUIModule ctor, owned at
// C_GUIModule+0x30; module message 0 runs its setup sub_180C3C314, module Deinit
// removes the input listener (pInput global qword_18492D840 vf[+0x20](this+0x08)) and
// deletes it via I_UIActionHintManager slot [5].
//
// Tracks the ACTIVE INPUT DEVICE CLASS for button-hint glyphs: OnInputEvent
// (sub_1811671E4) classifies each event (KB/M=1, gamepad=2, "p"-prefixed pad=16 --
// XInput vs PS pad), compares with the +0x28 cached state object and pushes changes;
// GetKeyLabel (sub_181519B88) maps raw key names to "@ui_control_uc_%d"/"@ui_key_%s"
// localization ids.
//
// [FUNDAMENTAL vs KCD1] KCD1 C_UIActionHintManager was a CUIManager-side element
// helper (ActionHint +0x60 block); the KCD2 one is a module-owned device watcher
// implementing a playermodule interface, with a C_Signal instead of element events.

namespace wh::guimodule {

class C_UIActionHintManager
    : public wh::playermodule::I_UIActionHintManager
    , public Offsets::IInputEventListener
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIActionHintManager;
    wh::shared::C_Signal<> m_deviceChangedSignal;  // +0x10  empty-sentinel unk_1856692D0 (arg signature UNVERIFIED)
    uint32_t m_deviceClass;                        // +0x20  ctor 0; returned by I_UIActionHintManager[0]
    uint32_t _pad24;                               // +0x24
    void*    m_28;                                 // +0x28  ctor-untouched; setup sub_180C3C314 caches framework-singleton subobject: qword_18549D388[+8]->vf[0xE0]() (subsystem ptr; pointee type UNVERIFIED)
    ICVar*   m_pDeviceState;                       // +0x30  "wh_ui_InputDevice" CVar (input-device class 1=KB/M 2=pad 16=PS-pad); setup sub_180C3C314 GetCVar+SetOnChangeCallback(sub_182BBA0E0)+Set(2)+GetIVal; OnInputEvent GetIVal()==vf+0x10 / Set(cls)==vf+0x38
};
static_assert(sizeof(C_UIActionHintManager) == 0x38, "C_UIActionHintManager must be 0x38");

}  // namespace wh::guimodule
