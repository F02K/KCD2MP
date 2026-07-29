#pragma once
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::playermodule::I_UIGameOver -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 5 slots.
// -----------------------------------------------
// RTTI .?AVI_UIGameOver@playermodule@wh@@ (TD 0x184CE1C00). Player-module face of the
// game-over screen. C_UIGameOver implements it @+0x58 (vtable 0x183C33A48) and
// publishes the subobject into the playermodule object: *(*(ctx+0x128)+0x48)+0x08 =
// &gameOver (Init sub_180C3B9E4). 2 methods + RTTR trio [2..4] (0x18213A8D8/
// 0x18213A5FC/0x18213A890). No virtual dtor slot.
//   [0] 0x182BBB120  Show: copies the payload, SetVisible(true) on the flash element
//       (IUIElement vf+0xE0), flash "Show"(text, mode), kicks a module fader op
//       (module+0x58 FaderController vf[+0x58]) and keeps its handle @owner+0xA0
//   [1] 0x182BB9A78  Hide: cancels the fader handle (vf[+0x10](0)) + teardown
// Names coined from behavior.

namespace wh::playermodule {

class I_UIGameOver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIGameOver;
    virtual void Show(const CryStringT<char>& text, int mode, void* pPayload) = 0;  // [0] 0x182BBB120 (param roles partially UNVERIFIED)
    virtual void Hide() = 0;                                                        // [1] 0x182BB9A78
    // RTTR trio [2..4] (impl-vtable thunks 0x18213A8D8/0x18213A5FC/0x18213A890, -0x58).
    // I_UIGameOver itself is NOT rttr-registered and appears in no base list
    // (C_UIGameOver's macro is RTTR_ENABLE(C_UIBase)). Slots binary-proven; macro
    // spelling is the inferred idiom.
    RTTR_ENABLE()
};
static_assert(sizeof(I_UIGameOver) == 8);

}  // namespace wh::playermodule
