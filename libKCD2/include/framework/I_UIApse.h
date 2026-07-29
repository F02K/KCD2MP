#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"
#include "framework/E_UIApseView.h"

// -----------------------------------------------
// wh::framework::I_UIApse -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 19 slots.
// -----------------------------------------------
// RTTI .?AVI_UIApse@framework@wh@@ (TD 0x184C92A98). The framework-side facade of the
// Apse menu hub: C_UIApse implements it at subobject +0x10 (impl vtable 0x183ED91B0);
// the standalone (pure) vtable is 0x183ED9320: [0] dtor 0x182AFDAC8, [1..15] _purecall,
// [16..18] rttr trio (get_type 0x182B03A04 / get_ptr 0x1805F5DA0 / get_derived_info
// 0x182B0391C).
//
// Method roles UNVERIFIED (no strings; C_UIApse impl bodies not traced this wave --
// impl addrs [1..15]: 0x181F4AA00, 0x180C44F48, 0x182AFEEB8, 0x181A73520, 0x180C42848,
// 0x180C42D28, 0x1819A6CF0, 0x182AFF600, 0x182AFF550, 0x182AFF6B0, 0x180B68E78,
// 0x1811A07D0, 0x182AFEA04, 0x1809E5B50, 0x182B009D8). Declared as coined _vfNN slots;
// signatures default to void() until traced.

namespace wh::framework {

class I_UIApse {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIApse;
    virtual ~I_UIApse();                                    // [0] 0x182AFDAC8 (standalone)
    virtual void _vf1() = 0;                                // [1]  role UNVERIFIED
    virtual void _vf2() = 0;                                // [2]  role UNVERIFIED
    virtual void _vf3() = 0;                                // [3]  role UNVERIFIED
    virtual void _vf4() = 0;                                // [4]  role UNVERIFIED
    virtual void _vf5() = 0;                                // [5]  role UNVERIFIED
    virtual void _vf6() = 0;                                // [6]  role UNVERIFIED
    // Set the active Apse view: writes the C_ModelProperty<E_UIApseView> at C_UIApse+0x28 and
    // fires its change signal.  force != 0 forces the transition; flag 0 = committed.
    virtual void SetApseView(E_UIApseView::Type view, char force, char flag) = 0;   // [7] 0x1819A6CF0
    virtual void _vf8() = 0;                                // [8]  role UNVERIFIED
    virtual void _vf9() = 0;                                // [9]  role UNVERIFIED
    virtual void _vf10() = 0;                               // [10] role UNVERIFIED
    virtual void _vf11() = 0;                               // [11] role UNVERIFIED
    virtual void _vf12() = 0;                               // [12] role UNVERIFIED
    virtual void _vf13() = 0;                               // [13] role UNVERIFIED
    virtual void _vf14() = 0;                               // [14] role UNVERIFIED
    virtual void _vf15() = 0;                               // [15] role UNVERIFIED
    RTTR_ENABLE()  // [16..18]: get_type 0x182B03A04, get_derived 0x182B0391C; baseless; in C_UIApse's RTTR_ENABLE(C_UIBase, I_UIApse)
};
static_assert(sizeof(I_UIApse) == 8);

}  // namespace wh::framework
