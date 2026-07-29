#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_StealthActionType (SAT_*) -- stealth-attack eligibility
// verdict / armed state (KCD2 WHGame.dll 1.5.6, kd7u).  int32 at
// C_CombatActorStealth+0x70.
// -----------------------------------------------
// No named RTTI enum exists; names recovered from the ToString string table
// @0x183ECC2C0.. (registered in C_ScriptBindActor ctor sub_181289FD0), values
// pinned by the Can-checker sub_18072F2A8 + sub_181E3DFD0 + callback writers.

namespace wh::combatmodule {

enum class E_StealthActionType : int32_t {
    SAT_Undefined            = 0,   // ineligible (checker return 0; str 0x183ecc2d8)
    SAT_KillDisabled         = 1,   // sub_181E3DFD0: (3!=3)+1 (str 0x183ecc2c0)
    SAT_KnockoutDisabled     = 2,   // sub_181E3DFD0: (4!=3)+1 (str 0x183ecc2f8)
    SAT_KillEnabled          = 3,   // checker returns reqType 3 @0x18072f60a (str 0x183ecc2e8)
    SAT_KnockoutEnabled      = 4,   // cb1 arm writes 4 @0x181706c9b (str 0x183ecc330)
    SAT_KillDisabledNoDagger = 5,   // checker @0x18072f4fd (str 0x183ecc310)
};

}  // namespace wh::combatmodule
