#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatAutomationZoneState -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// C_CombatAutomationZoneChange::m_state (+0x1C1). Names from the inline decode in
// the debug dump sub_182768CB8 @0x182768D98 ("State = %s": 0->"idle",
// 1->"sequence", else "<invalid enum value>").

namespace wh::combatmodule {

enum class E_CombatAutomationZoneState : uint8_t {
    Idle     = 0,  // "idle"
    Sequence = 1,  // "sequence"
};

}  // namespace wh::combatmodule
