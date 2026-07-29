#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIModalDialogInfo -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xA8.
// -----------------------------------------------
// RTTI .?AVC_UIModalDialogInfo@guimodule@wh@@ (TD 0x184C9EAA0).
// Bases: C_UIFlashBase @+0x00 (COL 0x413A6F8), IUIElementEventListener @+0x10
// (COL 0x413A720). vtables: primary 0x183A813A8 (14 slots; overrides [1] Init
// 0x180C415A0), listener 0x183A81360 (8). ctor sub_180BC842C.
// [G4a] Replaces the G3b opaque byte-blob shell -- the C_UIFlashBase base is PROVEN
// from the two COLs above.
//
// One-button modal info dialog, flash element "ApseModalDialog" (shared movie).
// Init registers ONE inbound dispatcher: "OnInfoDialogConfirmClicked" -> sub_182B13C3C
// (void()), which invokes m_onConfirm. The callback is set by whoever SHOWS the dialog.
// Embedded BY VALUE: C_UIApse+0x2770, C_UIApsePlayer+0x2D0, C_UIInventoryBase+0x4E8,
// C_UIQuestLog+0x378, C_UIHudMessages+0x60.

namespace wh::guimodule {

class C_UIModalDialogInfo : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIModalDialogInfo;
    std::function<void()> m_onConfirm;   // +0x58  (0x40) invoked by "OnInfoDialogConfirmClicked" (role from dispatcher name)
    void*                 m_pDelegate;   // +0x98  dim-delegate (owner iface subobject): show sub_182B14C6C stores the arg here + calls vf[0](0) to dim; single-callback layout shifts it up from Question's +0xD8; iface class UNRESOLVED
    bool                  m_isOpen;      // +0xA0  dialog-open flag: show sub_182B14C6C sets =1 (== Question::m_isOpen); gates the inbound forwarders
    uint8_t               _padA1[7];     // +0xA1
};
static_assert(sizeof(C_UIModalDialogInfo) == 0xA8, "C_UIModalDialogInfo must be 0xA8");
static_assert(offsetof(C_UIModalDialogInfo, m_onConfirm) == 0x58, "confirm callback at 0x58");

}  // namespace wh::guimodule
