#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::guimodule::I_FaderController -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 8 slots.
// -----------------------------------------------
// RTTI .?AVI_FaderController@guimodule@wh@@ (TD 0x184CB0BA0). Screen-fade facade,
// implemented by C_FaderController (@+0x00, vtable 0x183A97220). Fade requests are
// keyed by a 64-bit FNV-1a id. NO virtual dtor (verify pass) -- slot [0] 0x180C09080
// is the FULL fade request (map insert @+0xC8 under SRWLOCK +0x148, stores a4 @+0x170);
// [1] 0x180C08E84 forwards to [0] appending the default duration (+0x1B4); [2]
// RequestFade 0x180C09498; [3] 0x180C09488 forwards to [2] (+0x1B0); [4] CancelFade
// 0x180C09710. Names [2]/[4] behavior-coined.

namespace wh::guimodule {

class I_FaderController {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_FaderController;
    virtual void _vf0(void* a2, void* a3, int a4) = 0;       // [0] full fade request 0x180C09080 -- NOT a dtor
    virtual void _vf1() = 0;                                 // [1] forwards to [0] with default duration
    virtual void RequestFade(uint64_t fadeId, int a3) = 0;   // [2] name coined; moves the id into the active-fade map
    virtual void _vf3() = 0;                                 // [3] role UNVERIFIED
    virtual void CancelFade(uint64_t fadeId) = 0;            // [4] name coined; removes the id from the active-fade map
    RTTR_ENABLE()  // [5..7] = C_FaderController's trio (0x182B3EB7C / 0x182B3E650); I_FaderController NOT registered -- macro INFERRED idiom
};
static_assert(sizeof(I_FaderController) == 8);

}  // namespace wh::guimodule
