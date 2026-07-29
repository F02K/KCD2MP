#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::I_UIHudInteractiveDialog -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 22 slots.
// -----------------------------------------------
// RTTI .?AVI_UIHudInteractiveDialog@dialogmodule@wh@@ (TD 0x184CCFC48). Dialog-module
// face of the in-dialog HUD (response wheel/list, sequence state). C_UIHudInteractiveDialog
// implements it @+0x58 (vtable 0x183C30518) and publishes the subobject into the
// dialogmodule object: *(*(ctx+0x150)+0x10)+0x118 = &dlg (Init sub_180C3CAE8).
// Shape: [0] deleting-dtor thunk 0x18213A494, [1..18] 18 methods, [19..21] RTTR trio
// (0x18213A668/0x18213A5FC/0x18213A5B4). Sampled bodies only; rest is G4 scope.
//   [1] 0x1808BCCE8  big response/topic flash call (str, bool, bool, str, int, int*, bool)
//   [2] 0x180565E08  pushes the queued response list (owner vector @+0x68, stride 0x28
//       records {int, str, int, str, int duration*0.1}) to flash
//   [3] 0x1808BCE18  flash "SetActiveSequenceChanged"(int, int)
// NOTE [14] and [17] share one impl (0x181F7A440) -- two interface methods with the
// same body, kept as separate slots.

namespace wh::dialogmodule {

class I_UIHudInteractiveDialog {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIHudInteractiveDialog;
    virtual ~I_UIHudInteractiveDialog();                          // [0]
    virtual void _vf01() = 0;                                     // [1]  0x1808BCCE8  response/topic call; role UNVERIFIED
    virtual void PushResponses(bool a2) = 0;                      // [2]  0x180565E08  (name coined)
    virtual void SetActiveSequenceChanged(int a2, int a3) = 0;    // [3]  0x1808BCE18  flash "SetActiveSequenceChanged"
    virtual void _vf04() = 0;                                     // [4]  0x180557390  role UNVERIFIED
    virtual void _vf05() = 0;                                     // [5]  0x182B8EAF4  role UNVERIFIED
    virtual void _vf06() = 0;                                     // [6]  0x182B8F8A4  role UNVERIFIED
    virtual void _vf07() = 0;                                     // [7]  0x18056576C  role UNVERIFIED
    virtual void _vf08() = 0;                                     // [8]  0x182B8E980  role UNVERIFIED
    virtual void _vf09() = 0;                                     // [9]  0x1805657A4  role UNVERIFIED
    virtual void _vf10() = 0;                                     // [10] 0x182B8F108  role UNVERIFIED
    virtual void _vf11() = 0;                                     // [11] 0x1817D53A4  role UNVERIFIED
    virtual void _vf12() = 0;                                     // [12] 0x181F7A330  role UNVERIFIED
    virtual void _vf13() = 0;                                     // [13] 0x181F78530  role UNVERIFIED
    virtual void _vf14() = 0;                                     // [14] 0x181F7A440  role UNVERIFIED (shared with [17])
    virtual void _vf15() = 0;                                     // [15] 0x181F7A880  role UNVERIFIED
    virtual void _vf16() = 0;                                     // [16] 0x181F78610  role UNVERIFIED
    virtual void _vf17() = 0;                                     // [17] 0x181F7A440  role UNVERIFIED (shared with [14])
    virtual void _vf18() = 0;                                     // [18] 0x180566258  role UNVERIFIED
    RTTR_ENABLE()  // [19..21] thunks -0x58; baseless, get_type 0x182B927C8
};
static_assert(sizeof(I_UIHudInteractiveDialog) == 8);

}  // namespace wh::dialogmodule
