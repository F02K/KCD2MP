#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatAutomationWeaponRequest -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// C_CombatAutomationWeapons::m_requestedWeapon (+0x74). Enumerator names
// BINARY-VERBATIM from the ToString sub_1827690D4 (the debug dump sub_182768BE8
// formats "WEAPONS: Req %s" @0x182768C2D).

namespace wh::combatmodule {

enum class E_CombatAutomationWeaponRequest : uint8_t {
    None      = 0,  // "none"
    Melee     = 1,  // "melee"
    Missile   = 2,  // "missile"
    Oversized = 3,  // "oversized"
    Undefined = 4,  // "undefined"
};

}  // namespace wh::combatmodule
