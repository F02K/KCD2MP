#pragma once
#include <cstdint>
#include "C_CombatAutomationAction.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationCombo : C_CombatAutomationAction -- KCD2 1.5.6 (kd7u).  sizeof 0x78.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x260 (ctor sub_1810EEEDC; vtable 0x183B0A048; debug dump
// sub_1827681DC "COMBO: ..."). m_targetWuidLo/Hi form an UNALIGNED 8-byte WUID-shaped -1/-1 pair at
// +0x64 (cannot be typed wh::framework::WUID without breaking alignment).

namespace wh::combatmodule {

struct S_ComboStepDesc;   // combo-step row (floats @+0/+4/+8, name id @+0xC via sub_182764D48)

class C_CombatAutomationCombo : public C_CombatAutomationAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationCombo;
    const char* GetName() const override { return "AutomationCombo"; }   // [3] 0x181A7E1A0
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::Combo; }                    // [10] 0x181A74D00

    uint64_t m_rngA;               // +0x30  LCG-seeded (consumer not located)
    uint64_t m_rngB;               // +0x38  LCG-seeded (consumer not located)
    uint64_t m_comboProbRng;       // +0x40  LCG state feeding m_comboProbRoll
    float    m_comboProbRoll;      // +0x48  [0,1) roll; "ComboProb"
    int32_t  m_field4C;            // +0x4C  (ctor 0; no reader observed)
    int32_t  m_field50;            // +0x50  (ctor 0; no reader observed)
    int32_t  m_field54;            // +0x54  (ctor 0; no reader observed)
    int32_t  m_comboDefId;         // +0x58  combo-definition id (debug: key into the global 88B combo-def table -> "Combo: %s")
    int32_t  m_selectedSteps;      // +0x5C  "SelSteps: %d"
    int32_t  m_currentStep;        // +0x60  init 1; "CurStep %d"
    int32_t  m_targetWuidLo;       // +0x64  init -1  (unaligned WUID-shaped pair; read-site not found)
    int32_t  m_targetWuidHi;       // +0x68  init -1
    uint32_t _pad6C;               // +0x6C
    S_ComboStepDesc* m_pCurrentComboDesc;   // +0x70  weak; debug reads floats @+0/+4/+8 + name @+0xC
};
static_assert(sizeof(C_CombatAutomationCombo) == 0x78, "C_CombatAutomationCombo must be 0x78");

}  // namespace wh::combatmodule
