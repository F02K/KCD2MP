#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatAutomationDirectorState -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// C_CombatAutomationDirector::m_state (+0x34). Enumerator names BINARY-VERBATIM
// from the ToString sub_182769FAC (the debug dump sub_182768708 formats
// "DIR: State %s" from it @0x18276882D).

namespace wh::combatmodule {

enum class E_CombatAutomationDirectorState : int32_t {
    Idle                     = 0,  // "idle"
    HuntAttack               = 1,  // "huntAttack"
    MissileWeapon            = 2,  // "missileWeapon"
    Wait                     = 3,  // "wait"
    SlowPursuit              = 4,  // "slowPursuit"
    Uncertain                = 5,  // "uncertain"
    WaitForTargetReturnToNMN = 6,  // "waitForTargetReturnToNMN"
    WaitForPathToTarget      = 7,  // "waitForPathToTarget"
    Last                     = 8,  // "last" (count sentinel)
};

}  // namespace wh::combatmodule
