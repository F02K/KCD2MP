#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::playermodule::I_UIHudHints -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 10 slots.
// -----------------------------------------------
// RTTI .?AVI_UIHudHints@playermodule@wh@@ (TD 0x184CD1010). Player-module face of the
// interaction-hint HUD (crosshair hints, hold-progress). C_UIHudHints implements it
// @+0x58 (vtable 0x183C341B8) and publishes the subobject into the playermodule object:
// *(*(ctx+0x128)+0x58)+0x130 = &hints (Init sub_180C3D434). 7 methods + RTTR trio
// [7..9] (0x18213A650/0x18213A5FC/0x18213A59C). No virtual dtor slot.
// Sampled bodies: [1] 0x180C4488C flash "SetProgress"(clamp(pct*100,-100,100), a3, a4);
// [2] 0x1812C1544 flash call (CryStringT, int, int). Others UNVERIFIED (G4 scope).

namespace wh::playermodule {

class I_UIHudHints {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIHudHints;
    virtual void _vf0() = 0;                                        // [0] 0x1803C163C  role UNVERIFIED
    virtual void SetProgress(float progress, int a3, int a4) = 0;   // [1] 0x180C4488C  flash "SetProgress" (pct clamped to [-100,100])
    virtual void _vf2() = 0;                                        // [2] 0x1812C1544  flash (CryStringT,int,int); role UNVERIFIED
    virtual void _vf3() = 0;                                        // [3] 0x182B8F8E8  role UNVERIFIED
    virtual void _vf4() = 0;                                        // [4] 0x180C4554C  role UNVERIFIED
    virtual void _vf5() = 0;                                        // [5] 0x1816CF87C  role UNVERIFIED
    virtual void _vf6() = 0;                                        // [6] 0x1819C6948  role UNVERIFIED
    // RTTR trio [7..9] (impl-vtable thunks 0x18213A650/0x18213A5FC/0x18213A59C, -0x58).
    // I_UIHudHints itself is NOT rttr-registered and appears in no base list --
    // C_UIHudHints's macro is RTTR_ENABLE(C_UIBase, I_UIDragAndDropViewer). The trio
    // slots here are binary-proven; the macro spelling is the inferred idiom.
    RTTR_ENABLE()
};
static_assert(sizeof(I_UIHudHints) == 8);

}  // namespace wh::playermodule
