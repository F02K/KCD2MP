#pragma once
#include <cstdint>
#include "C_CombatAutomationAction.h"
#include "E_CombatAutomationDirectorState.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationDirector : C_CombatAutomationAction -- KCD2 1.5.6 (kd7u).  sizeof 0x90.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x50 (ctor inlined in sub_1810EF2DC; vtable 0x183B0A408).
// Field roles recovered from the debug-state dump override sub_182768708 ("Pursuit/State/Wait/
// LastU.able/Limit/U.edFilter/IsAimed/Estimating"). +0x40/+0x48 use 32-bit tick timestamps
// (sub_1809D382C), a different clock than CTimeValue.

namespace wh::combatmodule {

class C_CombatAutomationDirector : public C_CombatAutomationAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationDirector;
    const char* GetName() const override { return "AutomationDirector"; }   // [3] 0x181A7E1C0
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::Director; }                        // [10] 0x181A72470

    bool       m_pursuit;              // +0x30  "Pursuit: %s"
    uint8_t    _pad31[3];              // +0x31
    E_CombatAutomationDirectorState m_state;  // +0x34  "DIR: State %s" (ToString sub_182769FAC)
    CTimeValue m_waitUntil;            // +0x38  init -100000; "Wait %.2fs"
    int32_t    m_lastUpdatableTick;    // +0x40  32-bit tick timestamp; "LastU.able %.2fs"
    uint32_t   _pad44;                 // +0x44  (ctor-zeroed; role unresolved)
    int64_t    m_limit;                // +0x48  >0 gate; low32 = tick timestamp; "Limit %.2fs"
    int64_t    m_field50;              // +0x50  (ctor 0; no reader observed)
    CTimeValue m_filterA;              // +0x58  init -100000; "U.edFilter"
    CTimeValue m_filterB;              // +0x60  init -100000
    CTimeValue m_time68;               // +0x68  init -100000 (role unresolved)
    CTimeValue m_aimedSince;           // +0x70  init -100000; "IsAimed %.2fs"
    CTimeValue m_time78;               // +0x78  init -100000 (not in the debug dump)
    CTimeValue m_estimatingSince;      // +0x80  init -100000; "Estimating %f s"
    int64_t    m_field88;              // +0x88  (ctor 0; no reader observed)
};
static_assert(sizeof(C_CombatAutomationDirector) == 0x90, "C_CombatAutomationDirector must be 0x90");

}  // namespace wh::combatmodule
