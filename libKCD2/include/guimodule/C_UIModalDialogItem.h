#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIModalDialogItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI .?AVC_UIModalDialogItem@guimodule@wh@@ (TD 0x184C9EBD8). C_UIFlashBase leaf
// (vfptrs @+0x00/+0x10) -- SIBLING of C_UIModalDialogQuestion (same one-callback-then-
// state shape). No standalone ctor observed: constructed INLINE by the C_UIHudMessages
// ctor (sub_180BC9680 @0x180bc96f7.., EMBEDDED there at +0x60) bound to the
// "ApseModalDialog" flash element; Init driven by the owner as item->vf[1](module)
// (@0x180ec76a4). Layout pinned by the inline init: std::function _Impl slot zeroed at
// +0x90 (= +0x58+0x38), tail qwords +0x98/+0xA0 and flag byte +0xA8 zeroed; next owner
// member at +0x110 pins sizeof = 0xB0.
// OPAQUE beyond sizes: callback signature and tail roles UNVERIFIED (G4 modal wave).

namespace wh::guimodule {

class C_UIModalDialogItem : public C_UIFlashBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIModalDialogItem;
    std::function<void()> m_callback58;   // +0x58  (0x40) [role/signature UNVERIFIED]
    uint64_t              m_unk98;        // +0x98  ctor 0 [role UNVERIFIED]
    void*                 m_pFaderHandle; // +0xA0  owning smart-handle to a "ModalDialog" fade obj: show sub_182B14E80 move-assigns via sub_180C08F90(this+0xA0, module->m_pFaderController->vtbl[0](&out,"ModalDialog",0)); pointee released via vf[1](bool); class UNRESOLVED
    bool                  m_flagA8;       // +0xA8  ctor 0 [dialog-open flag? UNVERIFIED]
    uint8_t               _padA9[7];      // +0xA9
};
static_assert(sizeof(C_UIModalDialogItem) == 0xB0, "C_UIModalDialogItem must be 0xB0");
static_assert(offsetof(C_UIModalDialogItem, m_unk98) == 0x98, "tail at 0x98");

}  // namespace wh::guimodule
