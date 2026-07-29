#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_StealthKillResult -- stealth takedown outcome (KCD2
// WHGame.dll 1.5.6, kd7u).  u8 at C_CombatActorStealth+0x9C (default 3 = fail).
// -----------------------------------------------
// Names verbatim from the message writer sub_1827C7C3C switch (@0x1827c7caf..
// 0x1827c7cca; "<invalid enum value>" default).  Broadcast as message
// "combat:stealthKillResult" (@0x183e5ddd8), payload "attacker(%lld),
// result('%s')".  The fail path (3) is what C_RPGCombat slot 51 sub_182CFBFE4
// branches on to alert the victim.

namespace wh::combatmodule {

enum class E_StealthKillResult : uint8_t {
    success                  = 0,
    successWithPerfectBlock  = 1,
    successWithShout         = 2,
    fail                     = 3,   // default/reset value
};

}  // namespace wh::combatmodule
