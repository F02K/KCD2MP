#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashBase.h"
#include "dialogmodule/I_UIHudInteractiveDialog.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudInteractiveDialog -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xA0.
// -----------------------------------------------
// RTTI .?AVC_UIHudInteractiveDialog@guimodule@wh@@ (TD 0x184CCFBD0).
// Bases: C_UIFlashBase @+0x00 (COL 0x4135F68), IUIElementEventListener @+0x10
// (COL 0x4135F90), dialogmodule::I_UIHudInteractiveDialog @+0x58 (COL 0x4135FB8).
// vtables: primary 0x183C30478 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183C30430 (8), dialog 0x183C30518 (22). ctor sub_181876784; rttr
// as_std_shared_ptr creator sub_1818766F8 (alloc 0xA0).
//
// The in-dialog HUD (response list, sequence state, dialog timers). Init [1]
// sub_180C3CAE8: base Init, publishes the I_UIHudInteractiveDialog subobject into the
// dialogmodule object (*(*(ctx+0x150)+0x10)+0x118 = this+0x58). The dialog module
// drives everything through the 22-slot interface (see its header); PushResponses [2]
// walks m_responses and boxes each 0x28 record {int id, CryStringT text, int, CryStringT,
// int duration*0.1} into SUIArguments.

namespace wh::guimodule {

class C_UIHudInteractiveDialog : public C_UIFlashBase,
                                 public wh::dialogmodule::I_UIHudInteractiveDialog {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudInteractiveDialog;
    // Queued dialog-response record (0x28 stride from the PushResponses walk
    // @0x180565eac..0x180565f11; field layout beyond the boxing order UNVERIFIED).
    struct S_Response { uint8_t _raw[0x28]; };

    // I_UIHudInteractiveDialog overrides (sampled).
    void PushResponses(bool a2) override;                      // [I2] 0x180565E08
    void SetActiveSequenceChanged(int a2, int a3) override;    // [I3] 0x1808BCE18

    bool     m_flag60;                    // +0x60  ctor 0 (role UNVERIFIED)
    uint8_t  _pad61[7];                   // +0x61
    std::vector<S_Response> m_responses;  // +0x68  queued responses (PushResponses walk)
    uint64_t m_80;                        // +0x80  ctor 0 (role UNVERIFIED)
    uint64_t m_88;                        // +0x88  ctor 0 (role UNVERIFIED)
    uint64_t m_90;                        // +0x90  ctor 0 (role UNVERIFIED)
    int32_t  m_98;                        // +0x98  ctor 0 (role UNVERIFIED)
    uint8_t  _pad9C[4];                   // +0x9C
};
static_assert(sizeof(C_UIHudInteractiveDialog) == 0xA0, "C_UIHudInteractiveDialog must be 0xA0");
static_assert(offsetof(C_UIHudInteractiveDialog, m_responses) == 0x68, "response vector at 0x68");

}  // namespace wh::guimodule
