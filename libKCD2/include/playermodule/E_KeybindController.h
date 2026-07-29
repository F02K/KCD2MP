#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_KeybindController -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : int32.
// -----------------------------------------------
// Controller column of the keybinds system: index into S_KeybindsSuperaction::m_controllers[3]
// and C_Keybinds::m_inputToSuperactions[3].  ENUM NAME INVENTED (no binary name); values and
// literals EVIDENCED by the name<->index helpers sub_180DA23A4 (name->idx @0x180DA23B7) /
// sub_180DA235C (idx->name @0x180DA2382): "keyboard"=0, "xboxpad"=1, "ps4pad"=2, else 3.
// Invalid (3) doubles as the no-rebind/parse-miss sentinel: C_Keybinds ctor arms
// m_rebindController = 3 (0x18124427E) and every mutator guards controller != 3.
// The change/conflict notifiers map index -> device bitflag 0->1, 1->2, 2->4
// (sub_182E18A10 / sub_182E188DC), matching the wh_ui_InputDevice CVar flags
// [1=keyboard+mouse, 2=XBOX controller, 4=PlayStation controller].

namespace wh::playermodule {

struct E_KeybindController {
    enum Type : int32_t {
        Keyboard = 0,   // "keyboard"
        XboxPad  = 1,   // "xboxpad"
        Ps4Pad   = 2,   // "ps4pad"
        Invalid  = 3,   // no-rebind sentinel / unknown controller name
    };
};

}  // namespace wh::playermodule
