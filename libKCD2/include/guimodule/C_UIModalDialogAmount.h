#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIModalDialogAmount -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xE8.
// -----------------------------------------------
// RTTI .?AVC_UIModalDialogAmount@guimodule@wh@@ (TD 0x184C9E9F0).
// Bases: C_UIFlashBase @+0x00, IUIElementEventListener @+0x10. vtables: primary
// 0x183A81528 (14 slots; overrides [1] Init 0x180C416AC), listener 0x183A814E0 (8).
//
// Amount-picker modal (e.g. item split/transfer counts), flash element "ApseModalDialog".
// Init registers: "OnAmountDialogConfirmClicked" -> sub_180B5BB38 and
// "OnAmountDialogCancelClicked" -> sub_182B138FC (both void()). Layout mirrors
// C_UIModalDialogQuestion (confirm/cancel role split inferred from the dispatcher
// NAMES, not traced from the show-side setters). Embedded BY VALUE in
// C_UIInventoryBase @+0x318.

namespace wh::guimodule {

class C_UIModalDialogAmount : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIModalDialogAmount;
    std::function<void()> m_onConfirm;   // +0x58  (0x40) "OnAmountDialogConfirmClicked" [role from name]
    std::function<void()> m_onCancel;    // +0x98  (0x40) "OnAmountDialogCancelClicked" [role from name]
    void*                 m_pDelegate;   // +0xD8  dim-delegate (owner iface subobject): show sub_1808EC58C stores the arg here + calls vf[0](0) to dim; undimmed via vf[1](0) on close -- identical slot/role to C_UIModalDialogQuestion::m_pDelegate; iface class UNRESOLVED
    bool                  m_flagE0;      // +0xE0  ctor 0 [dialog-open flag? UNVERIFIED]
    uint8_t               _padE1[7];     // +0xE1
};
static_assert(sizeof(C_UIModalDialogAmount) == 0xE8, "C_UIModalDialogAmount must be 0xE8");
static_assert(offsetof(C_UIModalDialogAmount, m_onCancel) == 0x98, "second callback at 0x98");

}  // namespace wh::guimodule
