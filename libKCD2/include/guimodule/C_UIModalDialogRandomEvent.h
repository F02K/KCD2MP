#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIModalDialogRandomEvent -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x128.
// -----------------------------------------------
// RTTI .?AVC_UIModalDialogRandomEvent@guimodule@wh@@ (TD 0x184C9EC10).
// Bases: C_UIFlashBase @+0x00 (COL 0x413A7E8), IUIElementEventListener @+0x10
// (COL 0x413A810) -- SIBLING of C_UIModalDialogQuestion, not derived from it (CHD).
// vtables: primary 0x183B8EF40 (14 slots; [1] Init = plain C_UIFlashBase::Init
// 0x180565D38, [4] Update 0x1805682EC own), listener 0x183B8EEF8 (8).
// No standalone ctor -- constructed INLINE by the C_UIMap ctor (sub_18146C3C4
// @0x18146c44a..), which binds it to the same "ApseModalDialog" flash element.
//
// The fast-travel random-event (ambush) prompt: I_UIFastTravel slot [4]
// (C_UIMap 0x182AFDE1C) forwards the event text/options into it via the show entry
// sub_182B15268.  OPAQUE SHELL: callback signatures + tail roles UNVERIFIED.

namespace wh::guimodule {

class C_UIModalDialogRandomEvent : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIModalDialogRandomEvent;
    std::function<void()> m_callback58;   // +0x58  (0x40) [role/signature UNVERIFIED]
    std::function<void()> m_callback98;   // +0x98  (0x40) [role/signature UNVERIFIED]
    std::function<void()> m_callbackD8;   // +0xD8  (0x40) [role/signature UNVERIFIED]
    bool                  m_isOpen;       // +0x118 dialog-open flag: set=1 by the show path sub_182B15268
                                          //        (the ambush prompt), cleared on close (close path not traced)
    uint8_t               _pad119[3];     // +0x119
    Vec3                  m_pos11C;       // +0x11C ctor (-1,-1,-1) [event position? UNVERIFIED]
};
static_assert(sizeof(C_UIModalDialogRandomEvent) == 0x128, "C_UIModalDialogRandomEvent must be 0x128");
static_assert(offsetof(C_UIModalDialogRandomEvent, m_pos11C) == 0x11C, "sentinel Vec3 at 0x11C");

}  // namespace wh::guimodule
