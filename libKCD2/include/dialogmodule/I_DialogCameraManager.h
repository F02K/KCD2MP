#pragma once

// -----------------------------------------------
// wh::dialogmodule::I_DialogCameraManager -- dialog camera manager interface
// (KCD2 1.5.6, kd7u).  Pure interface, 7 slots.
// -----------------------------------------------
// RTTI .?AVI_DialogCameraManager@dialogmodule@wh@@ (TD 0x184B4B920, abstract, no
// standalone COL). Its vtable is C_DialogCameraManager's primary 0x183E63CA8 (7 slots).
// Slot roles UNVERIFIED (derive from the 7 slot bodies when needed) -- mirrored for
// layout/slot count only.

namespace wh::dialogmodule {

class I_DialogCameraManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DialogCameraManager;
    virtual ~I_DialogCameraManager();   // [0]
    virtual void _vf1() = 0;
    virtual void _vf2() = 0;
    virtual void _vf3() = 0;
    virtual void _vf4() = 0;
    virtual void _vf5() = 0;
    virtual void _vf6() = 0;
};
static_assert(sizeof(I_DialogCameraManager) == 0x08);

}  // namespace wh::dialogmodule
