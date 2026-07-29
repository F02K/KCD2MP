#pragma once
#include <cstdint>
#include "C_CombatAutomationAction.h"
#include "S_ReactionWeights.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationDefense : C_CombatAutomationAction -- KCD2 1.5.6 (kd7u).  sizeof 0x50.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x118 (ctor sub_1810EF244; vtable 0x183B0A390; debug
// dump sub_1827683BC "DEF %s: #%d, NO/NB/PB/MS/Dodge/MoveRes"). The evaluate path (sub_180C56048)
// computes cumulative reaction weights, rolls m_rngState, and picks action 0/5/6/7/8.
// NOTE: this leaf uses the inherited base slot +0x28 (m_field28) as a lazy cached director pointer
// (sub_1807302CC reads/caches it).

namespace wh::combatmodule {

// S_ReactionWeights (reaction weight override, 5 floats / 0x14) is defined in its own header.

class C_CombatAutomationDefense : public C_CombatAutomationAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationDefense;
    const char* GetName() const override { return "AutomationDefense"; }   // [3] 0x181A7E1B0
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::Defense; }                       // [10] 0x181A72500

    // Install a reaction-weight override (the 5 floats are value-copied into an owned 20-byte
    // heap block; any prior block is freed first). Slots < 0 keep the RPG-computed weight;
    // slots >= 0 replace it (per-slot >=0 test in the evaluate path sub_180C56588). To force a
    // defender to NEVER block / perfect-block / master-strike / dodge, pass {noBlock=1,
    // normalBlock=0, perfectBlock=0, specialPerfectBlock=0, dodge=0}. sub_18194C270 -- reached
    // in-engine via the I_CombatAutomation secondary vtable slot [9] thunk 0x182766C04
    // (`add rcx,100h` onto m_defense @automation+0x118), mirroring KCD1's thunk sub_1804F83BC.
    // Forwarder in C_CombatAutomationDefense.cpp.
    void SetReactionWeights(const S_ReactionWeights& weights);

    // Drop the override; reactions revert to the RPG-computed weights. sub_18194C270(this, nullptr).
    void ClearReactionWeights();

    uint64_t m_field30;                    // +0x30  (ctor 0; no reader in ANY Defense method -- reserved/unused)
    S_ReactionWeights* m_pWeightOverride;  // +0x38  OWNED heap 20B; null = none (writer = SetReactionWeights sub_18194C270)
    uint64_t m_rngState;                   // +0x40  LCG state (reaction roll)
    bool     m_bUseBlockQueryModifier;     // +0x48  gates the extra weight modifier (sub_182766C10)
    uint8_t  _pad49[7];                    // +0x49
};
static_assert(sizeof(C_CombatAutomationDefense) == 0x50, "C_CombatAutomationDefense must be 0x50");

}  // namespace wh::combatmodule
