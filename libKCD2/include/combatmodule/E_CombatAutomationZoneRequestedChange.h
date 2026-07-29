#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatAutomationZoneRequestedChange -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// C_CombatAutomationZoneChange::m_requestedChange (+0x1C0). Enumerator names
// BINARY-VERBATIM from the ToString sub_181E3F1B0 (dump sub_182768CB8 formats
// "ReqChange: %s" @0x182768E67).

namespace wh::combatmodule {

enum class E_CombatAutomationZoneRequestedChange : uint8_t {
    None        = 0,  // "none"
    Sequence    = 1,  // "sequence"
    Visual      = 2,  // "visual"
    Restriction = 3,  // "restriction"
    Adapting    = 4,  // "adapting"
};

}  // namespace wh::combatmodule
