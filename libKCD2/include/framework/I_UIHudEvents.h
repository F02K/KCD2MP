#pragma once
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::framework::I_UIHudEvents -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 26 slots.
// -----------------------------------------------
// RTTI .?AVI_UIHudEvents@framework@wh@@ (TD 0x184CD0F18). Framework-side face of the
// centre-screen game-event feed (fancy events, skill checks, item transfers, common
// events, location banners, ...). C_UIHudEvents implements it @+0x60 (vtable
// 0x183C30768): 23 method slots + RTTR trio thunks [23..25] (0x18213A644/0x18213A608/
// 0x18213A590, all -0x60). No virtual dtor slot.
//
// Only sampled bodies are named; the rest are UNVERIFIED (kept as numbered slots for the
// G4 wave). Samples: [0] 0x180515124 flash "ShowItemTransfer"(guid-pair, count, dir);
// the class also exposes many of these as rttr methods (method_wrapper TDs with
// CryStringT/int/CryGUID/E_QuestProgress/E_ItemTransferDirection signatures).

namespace wh::framework {

class I_UIHudEvents {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIHudEvents;
    virtual void ShowItemTransfer(const void* pItemGuid, int count, int direction) = 0;  // [0] 0x180515124 flash "ShowItemTransfer" (params partially UNVERIFIED)
    virtual void _vf01() = 0;   // [1]  0x18051498C  role UNVERIFIED
    virtual void _vf02() = 0;   // [2]  0x1805146A4  role UNVERIFIED
    virtual void _vf03() = 0;   // [3]  0x182B8EC90  role UNVERIFIED
    virtual void _vf04() = 0;   // [4]  0x181122DA0  role UNVERIFIED
    virtual void _vf05() = 0;   // [5]  0x182B8EDEC  role UNVERIFIED
    virtual void _vf06() = 0;   // [6]  0x180C4BA1C  role UNVERIFIED
    virtual void _vf07() = 0;   // [7]  0x182B8EF28  role UNVERIFIED
    virtual void _vf08() = 0;   // [8]  0x180C4C674  role UNVERIFIED
    virtual void _vf09() = 0;   // [9]  0x1819570B4  role UNVERIFIED
    virtual void _vf10() = 0;   // [10] 0x182B8EFDC  role UNVERIFIED
    virtual void _vf11() = 0;   // [11] 0x180C4B53C  role UNVERIFIED
    virtual void _vf12() = 0;   // [12] 0x180C4C894  role UNVERIFIED
    virtual void _vf13() = 0;   // [13] 0x180C4B658  role UNVERIFIED
    virtual void _vf14() = 0;   // [14] 0x1819DD9D0  role UNVERIFIED
    virtual void _vf15() = 0;   // [15] 0x181F7AF60  role UNVERIFIED
    virtual void _vf16() = 0;   // [16] 0x180DBD96C  role UNVERIFIED
    virtual void _vf17() = 0;   // [17] 0x180A19374  role UNVERIFIED
    virtual void _vf18() = 0;   // [18] 0x180C3EEA4  role UNVERIFIED
    virtual void _vf19() = 0;   // [19] 0x182B8ED30  role UNVERIFIED
    virtual void _vf20() = 0;   // [20] 0x180ED4DFC  role UNVERIFIED
    virtual void _vf21() = 0;   // [21] 0x181F78420  role UNVERIFIED
    virtual void _vf22() = 0;   // [22] 0x180ED2C28  role UNVERIFIED
    RTTR_ENABLE()  // [23..25] thunks -0x60; baseless (creator 0x181784490); in C_UIHudEvents's RTTR_ENABLE(C_UIBase, I_UIHudEvents)
};
static_assert(sizeof(I_UIHudEvents) == 8);

}  // namespace wh::framework
