#pragma once

// -----------------------------------------------
// wh::dialogmodule::I_TestDialog -- dialog debug/introspection interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVI_TestDialog@dialogmodule@wh@@ (TD 0x184B4B7B8, abstract, no COL). Implemented
// by C_DialogInstance @+0x28 (vtable 0x183E63A68, 16 slots). Debug/introspection surface
// for the wh_dlg_TestMode / wh_dlg_TestPreselectedSequences test path; [4] maps a state
// enum to its display string. Slot roles otherwise UNVERIFIED; low cheat value. Mirrored
// for layout/slot count only.

namespace wh::dialogmodule {

class I_TestDialog {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_TestDialog;
    virtual ~I_TestDialog();   // [0]
    virtual void _vf1() = 0;
    virtual void _vf2() = 0;
    virtual void _vf3() = 0;
    virtual void _vf4() = 0;   // [4] state enum -> string
    virtual void _vf5() = 0;
    virtual void _vf6() = 0;
    virtual void _vf7() = 0;
    virtual void _vf8() = 0;
    virtual void _vf9() = 0;
    virtual void _vf10() = 0;
    virtual void _vf11() = 0;
    virtual void _vf12() = 0;
    virtual void _vf13() = 0;
    virtual void _vf14() = 0;
    virtual void _vf15() = 0;
};
static_assert(sizeof(I_TestDialog) == 0x08);

}  // namespace wh::dialogmodule
