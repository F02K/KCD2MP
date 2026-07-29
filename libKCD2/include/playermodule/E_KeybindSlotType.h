#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_KeybindSlotType -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : int32.
// -----------------------------------------------
// Per-controller visibility/editability of a superaction (S_KeybindsSuperactionSlots::m_type),
// parsed from the <superaction keyboard=|xboxpad=|ps4pad=> attribute of
// Libs/Config/keybindSuperactions.xml.  ENUM NAME INVENTED; values and the
// "readable"/"writeable" literals EVIDENCED in the parser sub_1808FB4CC
// (@0x1808FB79A/0x1808FB7B6 via attr matcher sub_180697E14); "hidden" or any unmatched
// value falls through to 0.  Writeable is the rebind gate: C_Keybinds::ChangeInput
// (0x182E181A4) rejects slots whose m_type != 2.

namespace wh::playermodule {

struct E_KeybindSlotType {
    enum Type : int32_t {
        Hidden    = 0,   // "hidden" (or unmatched attr) -- not shown in the keybinds page
        Readable  = 1,   // "readable" -- shown, not rebindable
        Writeable = 2,   // "writeable" -- rebindable in the UI (ChangeInput gate)
    };
};

}  // namespace wh::playermodule
