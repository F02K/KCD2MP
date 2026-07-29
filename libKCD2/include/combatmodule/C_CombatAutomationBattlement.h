#pragma once
#include <cstdint>
#include "C_CombatAutomationAction.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationBattlement : C_CombatAutomationAction -- KCD2 1.5.6 (kd7u).  sizeof 0x40.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x650 (ctor inlined in sub_1810EF2DC; vtable 0x183B0A318).
// The battlement configuration itself lives in the automation model block
// (S_CombatAutomationModels::battlementProp).

namespace wh::combatmodule {

class C_CombatAutomationBattlement : public C_CombatAutomationAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationBattlement;
    const char* GetName() const override { return "AutomationBattlement"; }  // [3] 0x181A7E180
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::Battlement; }                        // [10] 0x181A72890

    CTimeValue m_lastTime2;   // +0x30  init -100000 ("never")
    bool       m_flag38;      // +0x38  (ctor 0; role unresolved)
    uint8_t    _pad39[7];     // +0x39
};
static_assert(sizeof(C_CombatAutomationBattlement) == 0x40, "C_CombatAutomationBattlement must be 0x40");

}  // namespace wh::combatmodule
