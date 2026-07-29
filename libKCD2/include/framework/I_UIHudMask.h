#pragma once
#include "rttr/rttr_enable.h"
#include "framework/E_HudElements.h"

// -----------------------------------------------
// wh::framework::I_UIHudMask -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 5 slots.
// -----------------------------------------------
// RTTI .?AVI_UIHudMask@framework@wh@@ (TD 0x184CD0C40). The framework-side face of the
// HUD element visibility mask; sole observed implementor is guimodule C_UIHudMask
// (subobject @+0x58, vtable 0x183C13D60):
//   [0] deleting-dtor adjustor thunk 0x18213A4AC (-0x58 -> sub_182B8BE80)
//   [1] IsElementVisible 0x180555E04: tests bit id of the mask's 28-bit visibility set
//       (mask+0x150) AND queries the per-element source handle (mask+0x70+8*id) vf[2]()
//   [2..4] RTTR_ENABLE() trio adjustor thunks (get_type 0x18213A674 -> 0x180ED5608,
//       get_ptr 0x18213A5FC -> sub_1805F5DA0, get_derived_info 0x18213A5C0 -> 0x18191DCB4)
// Method name coined from behavior (no PDB).

namespace wh::framework {

class I_UIHudMask {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIHudMask;
    virtual ~I_UIHudMask();                                              // [0]
    virtual bool IsElementVisible(E_HudElements::Type element) const = 0;  // [1] 0x180555E04 (impl in C_UIHudMask)
    RTTR_ENABLE()  // [2..4] thunks -0x58; baseless, get_type 0x182B927E4
};
static_assert(sizeof(I_UIHudMask) == 8);

}  // namespace wh::framework
