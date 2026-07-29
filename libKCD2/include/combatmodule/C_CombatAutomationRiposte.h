#pragma once
#include <cstdint>
#include "C_CombatAutomationAction.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationRiposte : C_CombatAutomationAction -- KCD2 1.5.6 (kd7u).  sizeof 0x48.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x218 (ctor sub_1810EF048; vtable 0x183B0A138). The
// evaluate path (sub_180C5A834) rolls m_rngStateA against the S_AutomationVars riposte-chance
// threshold and triggers sub_181899D6C(owner) on success. No owned pointers.

namespace wh::combatmodule {

class C_CombatAutomationRiposte : public C_CombatAutomationAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationRiposte;
    const char* GetName() const override { return "AutomationRiposte"; }   // [3] 0x181A7E230
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::Riposte; }                      // [10] 0x181A74D10

    uint64_t m_rngStateA;     // +0x30  LCG state (riposte-chance roll)
    uint64_t m_rngStateB;     // +0x38  LCG state, advanced per tick
    float    m_randomValue;   // +0x40  cached [0,1) roll from m_rngStateB
    uint8_t  _pad44[4];       // +0x44
};
static_assert(sizeof(C_CombatAutomationRiposte) == 0x48, "C_CombatAutomationRiposte must be 0x48");

}  // namespace wh::combatmodule
