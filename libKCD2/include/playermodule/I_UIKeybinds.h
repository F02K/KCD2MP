#pragma once

// -----------------------------------------------
// wh::playermodule::I_UIKeybinds -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 2 slots.
// -----------------------------------------------
// RTTI .?AVI_UIKeybinds@playermodule@wh@@ (TD 0x184CD99C0). Player-module face of the
// keybinding menu page. C_UIKeybinds implements it @+0x58 (vtable 0x183A40BA0) and
// publishes the subobject into the playermodule object: *(*(ctx+0x128)+0x40)+0x1E8 =
// &keybinds (Init sub_1806127D4 / cleared by Deinit sub_182BA6F48).
//   [0] 0x182BA97C0  flash "KeybindsSetConflict"(int, str, int, ...)
//   [1] 0x182BA9B50  flash "KeybindsChangeInput"(int, str, int, keyLabel via the
//       "@ui_key_%s" helper sub_181519B40)
// No dtor, no rttr trio. Names from the flash call names; exact params UNVERIFIED.

namespace wh::playermodule {

class I_UIKeybinds {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIKeybinds;
    virtual void SetConflict(int a2, const CryStringT<char>& action, int a4) = 0;      // [0] 0x182BA97C0
    virtual void ChangeInput(int a2, const CryStringT<char>& action, int a4,
                             const CryStringT<char>& key) = 0;                         // [1] 0x182BA9B50
};
static_assert(sizeof(I_UIKeybinds) == 8);

}  // namespace wh::playermodule
