#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIModalDialogHealing -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x100.
// -----------------------------------------------
// RTTI .?AVC_UIModalDialogHealing@guimodule@wh@@ (TD 0x184C9EB98).
// Bases: C_UIFlashBase @+0x00, IUIElementEventListener @+0x10. vtables: primary
// 0x183D144F0 (14 slots; overrides [1] Init 0x18183D3AC), listener (8).
//
// Healing-item modal, flash element "ApseModalDialog". Init registers:
//   "OnHealingDialogConfirmClicked" -> sub_182B13B34 (void(), via sub_180613AE8)
//   "OnHealingDialogCancelClicked"  -> sub_182B1398C (void())
//   "OnGetHealEffect"               -> sub_182B13EC8 -- registered via a DIFFERENT
//     helper (sub_18183D498): a non-void dispatcher, likely returning the selected
//     heal effect to flash; signature UNVERIFIED.
// Embedded BY VALUE in C_UIApseInventory @+0x5E8 (size pinned by the next embed
// C_UIModalDialogCombine @+0x6E8; 0x18 larger than the Question shape -- the extra
// space holds the heal-effect delegate state). Member breakdown beyond the two
// callbacks UNVERIFIED.

namespace wh::guimodule {

class C_UIModalDialogHealing : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIModalDialogHealing;
    std::function<void()> m_onConfirm;    // +0x58  (0x40) "OnHealingDialogConfirmClicked" [role from name]
    std::function<void()> m_onCancel;     // +0x98  (0x40) "OnHealingDialogCancelClicked" [role from name]
    std::vector<wh::entitymodule::C_Item*> m_selectedItems;  // +0xD8  heal-item list; passed by-ref to confirm cb std::function<void(vector<C_Item*>&,uint,int)> (lambda sig 0x181F4B989)
    void*                 m_pHintOwner;   // +0xF0  caller-owned iface (module+0x20); opener 0x182B1452C stores it, close 0x182B11A8F calls vf[1](this,0) then nulls it
    bool                  m_isActive;     // +0xF8  dialog-open gate; every handler tests *(this+0xF8) (0x182B13ED8); set 1 by opener 0x182B1452C, 0 by close 0x182B1199F
    uint8_t               _padF9[7];      // +0xF9  pad to sizeof 0x100
};
static_assert(sizeof(C_UIModalDialogHealing) == 0x100, "C_UIModalDialogHealing must be 0x100");
static_assert(offsetof(C_UIModalDialogHealing, m_onCancel) == 0x98, "second callback at 0x98");

}  // namespace wh::guimodule
