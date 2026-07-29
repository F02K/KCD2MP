#pragma once
#include <cstdint>
#include "C_CombatAutomationAction.h"
#include "../framework/C_ModelContextProperty.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationGuard : C_CombatAutomationAction -- KCD2 1.5.6 (kd7u).  sizeof 0x98.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x4C8 (ctor sub_1810EF0DC, sub-init sub_1810EF168; vtable
// 0x183B0A1E8 -- GetName literally returns the binary's "AutomationGaurd" typo; debug dump
// sub_18276895C "GUARD: ...").

namespace wh::combatmodule {

// Explicit guard request (values from the debug dump decoder).
enum class E_CombatExplicitGuardRequest : uint8_t {
    None      = 0,
    ForceOn   = 1,
    ForceOff  = 2,
    FreeBlock = 3,
};

class C_CombatAutomationGuard : public C_CombatAutomationAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationGuard;
    const char* GetName() const override { return "AutomationGaurd"; }   // [3] 0x181A7E1E0 (sic -- binary typo)
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::Guard; }                     // [10] 0x181A72480

    int32_t    m_field30;            // +0x30  (ctor 0; role unresolved)
    float      m_cfg34;              // +0x34  init 5.0
    float      m_cfg38;              // +0x38  init 3.0
    float      m_cfg3C;              // +0x3C  init 1.0
    CTimeValue m_updateGuardTimer;   // +0x40  "UpdateGuardTimer %2.3fs"
    CTimeValue m_timer48;            // +0x48  init -100000 (role unresolved)
    int32_t    m_mode;               // +0x50  "Mode %d" (raw int in dump; NOT E_CombatExplicitGuardRequest [that is m_forced +0x88]; domain UNVERIFIED)
    bool       m_suppressed;         // +0x54  "Supp"
    uint8_t    _pad55[3];            // +0x55
    float      m_borderA;            // +0x58  "Border %.2f+.."
    float      m_borderB;            // +0x5C  ".. +%.2f"
    uint16_t   m_field60;            // +0x60  (ctor 0; role unresolved)
    uint8_t    m_field62;            // +0x62  (ctor 0; role unresolved)
    uint8_t    _pad63[5];            // +0x63
    wh::shared::C_ModelContextProperty<E_CombatExplicitGuardRequest, bool, 0>
               m_explicitRequest;    // +0x68  (0x20) vtable + C_Signal + state
    E_CombatExplicitGuardRequest m_forced;   // +0x88  "Forced"
    uint8_t    _pad89[7];            // +0x89
    CTimeValue m_timer90;            // +0x90  init -100000 (role unresolved)
};
static_assert(sizeof(C_CombatAutomationGuard) == 0x98, "C_CombatAutomationGuard must be 0x98");

}  // namespace wh::combatmodule
