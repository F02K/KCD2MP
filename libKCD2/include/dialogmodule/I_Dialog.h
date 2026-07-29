#pragma once
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::I_Dialog -- the big dialog-object interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVI_Dialog@dialogmodule@wh@@ (TD 0x184B4B830). Standalone abstract vtable
// 0x183E63D30 (37 slots): slots [1..33] are _purecall; only [0]/[34]/[35]/[36] carry base
// defaults ([34..36] = the RTTR_ENABLE() trio; the earlier "Get* accessors returning
// copied handles" read matches the trio's by-value returns). Implemented by
// C_DialogInstance @+0x18 (impl vtable 0x183E63B60). Known behavior on the impl:
//   [1]  getter
//   [19] gates C_PlayerDialogController creation ("IsPlayerDialog?" UNVERIFIED)
//   [34]/[35]/[36] RTTR trio (get_type/get_ptr/get_derived_info)
// Most slot roles (Start/Update/Stop/actor/camera queries) UNVERIFIED -- mirrored for
// layout/slot count only.

namespace wh::dialogmodule {

class I_Dialog {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Dialog;
    virtual ~I_Dialog();      // [0]
    virtual void _vf1() = 0;  // [1] getter
    virtual void _vf2() = 0;
    virtual void _vf3() = 0;
    virtual void _vf4() = 0;
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
    virtual void _vf16() = 0;
    virtual void _vf17() = 0;
    virtual void _vf18() = 0;
    virtual void _vf19() = 0;  // [19] gates PlayerDialogController creation [UNVERIFIED]
    virtual void _vf20() = 0;
    virtual void _vf21() = 0;
    virtual void _vf22() = 0;
    virtual void _vf23() = 0;
    virtual void _vf24() = 0;
    virtual void _vf25() = 0;
    virtual void _vf26() = 0;
    virtual void _vf27() = 0;
    virtual void _vf28() = 0;
    virtual void _vf29() = 0;
    virtual void _vf30() = 0;
    virtual void _vf31() = 0;
    virtual void _vf32() = 0;
    virtual void _vf33() = 0;
    RTTR_ENABLE()  // [34..36]: get_type 0x181E641D0, get_derived 0x181E64180
};
static_assert(sizeof(I_Dialog) == 0x08);

}  // namespace wh::dialogmodule
