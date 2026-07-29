#pragma once
#include <cstdint>
#include "C_CombatAutomationAction.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationMissile : C_CombatAutomationAction -- KCD2 1.5.6 (kd7u).  sizeof 0x60.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x5F0 (ctor sub_1810EEFB4; vtable 0x183B0A0C0; debug dump
// sub_182768AE8 "MISSILE: ...").

namespace wh::combatmodule {

class C_CombatAutomationMissile : public C_CombatAutomationAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationMissile;
    const char* GetName() const override { return "AutomationMissile"; }   // [3] 0x181A7E200
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::Missile; }                      // [10] 0x181A7D840

    int32_t    m_stateFlag;     // +0x30  (ctor 0; role tentative)
    float      m_aimDuration;   // +0x34  init 1.0; "aimDuration %.2f"
    float      m_prediction;    // +0x38  "prediction %.2f"
    float      m_rngRoll;       // +0x3C  [0,1) roll
    uint64_t   m_rng;           // +0x40  LCG state feeding m_rngRoll
    CTimeValue m_timer48;       // +0x48  init -100000 (role tentative)
    CTimeValue m_aimTimer;      // +0x50  "aim %.2f"
    CTimeValue m_timer58;       // +0x58  init -100000 (role tentative)
};
static_assert(sizeof(C_CombatAutomationMissile) == 0x60, "C_CombatAutomationMissile must be 0x60");

}  // namespace wh::combatmodule
