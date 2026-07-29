#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::I_InteractiveScenePlayer -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 9 slots.
// -----------------------------------------------
// RTTI .?AVI_InteractiveScenePlayer@guimodule@wh@@ (TD 0x184A8BA58). The interactive-
// scene / cutscene player facade: implemented by C_CutscenePlayer (@+0x00, vtable
// 0x183EE1F90) and C_InteractiveSceneManager (spine.md, @C_GUIModule+0x70).
// Shape: dtor + 8 methods, NO rttr trio. Method roles/signatures UNVERIFIED (coined
// _vfNN; C_CutscenePlayer impl addrs: [1] 0x182B388C0, [2] 0x182B37608, [3] 0x182B36DD4,
// [4] 0x182B38AD8, [5] 0x182B36B58, [6] 0x181A73320, [7] 0x182B38AA4, [8] 0x182B39094).

namespace wh::guimodule {

class I_InteractiveScenePlayer {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_InteractiveScenePlayer;
    virtual ~I_InteractiveScenePlayer();   // [0]
    virtual void _vf1() = 0;               // [1] role UNVERIFIED
    virtual void _vf2() = 0;               // [2] role UNVERIFIED
    virtual void _vf3() = 0;               // [3] role UNVERIFIED
    virtual void _vf4() = 0;               // [4] role UNVERIFIED
    virtual void _vf5() = 0;               // [5] role UNVERIFIED
    virtual void _vf6() = 0;               // [6] role UNVERIFIED
    virtual void _vf7() = 0;               // [7] role UNVERIFIED
    virtual void _vf8() = 0;               // [8] role UNVERIFIED
};
static_assert(sizeof(I_InteractiveScenePlayer) == 8);

}  // namespace wh::guimodule
