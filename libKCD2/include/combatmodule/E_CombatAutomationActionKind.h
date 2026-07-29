#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatAutomationActionKind -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// C_CombatAutomationAction::GetActionKind() [10] per-leaf constant (byte-verified
// "mov eax,imm; ret" stubs bound via each leaf vtable's slot 10). Names from each
// leaf's GetName() [3] literal ("Automation<Name>"; "AutomationGaurd" sic in binary).
// Gaps 0,5-9,13 have no leaf class in 1.5.6 (no dispatcher names them; likely
// reserved/removed since KCD1). C_CombatTemporaryAutomationGuardStarter is NOT a
// kind (different base, no GetActionKind slot).

namespace wh::combatmodule {

enum class E_CombatAutomationActionKind : int32_t {
    Director   = 1,   // stub 0x181A72470, vtable 0x183B0A408
    Guard      = 2,   // stub 0x181A72480 ("AutomationGaurd" sic)
    ZoneChange = 3,   // stub 0x181A72CF0
    Defense    = 4,   // stub 0x181A72500
    Attack     = 10,  // stub 0x181A74D20
    Riposte    = 11,  // stub 0x181A74D10
    Combo      = 12,  // stub 0x181A74D00
    Weapons    = 14,  // stub 0x181A7D850
    Missile    = 15,  // stub 0x181A7D840
    Battlement = 16,  // stub 0x181A72890
};

}  // namespace wh::combatmodule
