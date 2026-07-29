#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include "C_CombatActorObject.h"
#include "I_CombatComboManager.h"
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatComboManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xF0.
// -----------------------------------------------
// ctor sub_1810F09B0. Stored at C_CombatActor+0x3B0. MI (RTTI CHD): C_CombatActorObject (primary,
// mdisp 0) + I_CombatComboManager (secondary vtable @+0x10, mdisp 0x10). Own members from +0x18.
//
// [FUNDAMENTAL vs KCD1] Shrank 0x230 -> 0xF0: the combo transition-rule table and tracker map moved
// onto S_CombatActorState (the ctor only SUBSCRIBES to state signals/properties at owner->m_pState:
// +0, +0x48, +0x360, +0x480, +0x860, +0xC20, +0x910 and the actor's target-change vfunc +0x368).
// KCD1's 7 signals became 9 (C_Signal shrank 0x30 -> 0x10).

namespace wh::combatmodule {

class I_CombatActor;
class C_CombatComboData;
enum class E_CombatZoneId : int32_t;

// One tracked combo (0x18). RTTI .?AUComboState@C_CombatComboManager@combatmodule@wh@@ proves the
// name; built by the seeder sub_1808D1048 (stride 24: def@0, stepCount@8 = (def+72 - def+64)>>5,
// active@0xC, progress@0x10). The candidate lists inside S_ActiveCombo iterate the same 24-byte
// {ptr@0, u32@8, byte@0xC} shape -- treated as the same struct (unification inferred).
struct S_ComboState {
    C_CombatComboData* m_pComboDef;   // +0x00  combo definition row
    uint32_t           m_stepCount;   // +0x08  steps in the definition
    uint8_t            m_active;      // +0x0C  matched/active flag (iteration break condition)
    uint8_t            _pad0D[3];     // +0x0D
    uint32_t           m_progress;    // +0x10  current step progress (init 0)
    uint8_t            _pad14[4];     // +0x14
};
static_assert(sizeof(S_ComboState) == 0x18, "S_ComboState must be 0x18");

// One in-flight combo (0x28; iteration stride proven in sub_180C5C594 / sub_180C5C71C).
struct S_ActiveCombo {
    void*    __vftable;                        // +0x00  ComboState vtable 0x183B529A0 (RTTI ??_7ComboState@C_CombatComboManager@combatmodule@wh@@6B@; 2 vfuncs sub_181A72970/sub_180C60F24) -- struct is polymorphic, model as class
    int32_t  m_currentStep;                    // +0x08  ++ on advance
    uint8_t  _pad0C[4];                        // +0x0C  alignment pad before 8-aligned m_candidates (unwritten by emplace sub_1812C6028/sub_1812C5D3C; uncopied by compaction sub_180C5C71C)
    std::vector<S_ComboState> m_candidates;    // +0x10
};
static_assert(sizeof(S_ActiveCombo) == 0x28, "S_ActiveCombo must be 0x28");

class C_CombatComboManager : public C_CombatActorObject, public I_CombatComboManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatComboManager;

    // Force one CLEAN combo-step advance via the engine's own state-signal handler
    // (sub_180C5C52C -- the KCD2 rebuild of KCD1 sub_180602C34, invoked from the state-event
    // dispatcher sub_180911250 case 32). Touches ONLY this manager: appends the fresh combo
    // seed, per active combo re-filters candidates against the state's executed-attack-zone
    // property (+0x270) + m_lastComboZone, ++m_currentStep, compacts, emits m_sigComboAdvanced
    // (core sub_180C5C594). No faked hit, no damage. Behaviour deltas vs KCD1: (1) if
    // m_pendingAdvanceFlag (+0xE8) is set the engine RESETS the combo instead (sub_180C5E460);
    // (2) NEW dedup gate -- no-op when m_stepAppliedFlag (+0xEA) is already set, and the call
    // sets it (cleared again on the next state-id-4 event); (3) the core is gated on the
    // combat-settings int at g_combatSettings+0x188 (combos enabled); (4) takes only `this`
    // (KCD1 passed an unused emitter + a char gate). Forwarder in C_CombatComboManager.cpp.
    void ForceAdvanceStep();

    wh::shared::C_Signal<I_CombatActor&, bool>      m_onComboSlotChanged;  // +0x18  args tentative (desc unk_185666A00; could carry E_HandSlot)
    wh::shared::C_Signal<I_CombatActor&, int, bool> m_onComboStepChanged;  // +0x28  args tentative (desc unk_185666A70)
    wh::shared::C_Signal<I_CombatActor&>            m_sigComboAdvanced;    // +0x38  emit sub_180C5C594 tail
    wh::shared::C_Signal<I_CombatActor&>            m_sigComboReset;       // +0x48  emit sub_180C5E460
    wh::shared::C_Signal<I_CombatActor&, int>       m_signal_058;          // +0x58  (no emit site located)
    wh::shared::C_Signal<I_CombatActor&, int>       m_signal_068;          // +0x68  emit on dirty-slot flush (sub_180C5E538)
    wh::shared::C_Signal<I_CombatActor&, int>       m_signal_078;          // +0x78  emit in state-handler in-combo case (sub_180911250)
    wh::shared::C_Signal<I_CombatActor&, int>       m_signal_088;          // +0x88  emit on combo-continue flush (sub_180C5E538)
    wh::shared::C_Signal<I_CombatActor&, int>       m_signal_098;          // +0x98  (no emit site located)
    std::vector<S_ComboState>  m_comboSeeds;       // +0xA8  seeded from the global combo-def table (KCD1 m_state198)
    std::vector<S_ActiveCombo> m_activeCombos;     // +0xC0  in-flight combos (stride 0x28)
    int32_t  m_dirtySlotCounter;                   // +0xD8  !=0 -> emit m_signal_068 then reset
    int32_t  m_comboContCounter;                   // +0xDC  KCD1 +0x21C
    E_CombatZoneId m_comboFireGateZone;            // +0xE0  init -1 (KCD1 +0x220)
    E_CombatZoneId m_lastComboZone;                // +0xE4  init -1 (KCD1 +0x224)
    uint8_t  m_pendingAdvanceFlag;                 // +0xE8  deferred advance vs reset
    uint8_t  _padE9;                               // +0xE9
    uint8_t  m_stepAppliedFlag;                    // +0xEA
    uint8_t  m_lastSlotNotifyValue;                // +0xEB  cached last broadcast bool
    uint8_t  _padEC[4];                            // +0xEC
};
static_assert(sizeof(C_CombatComboManager) == 0xF0, "C_CombatComboManager must be 0xF0");
static_assert(offsetof(C_CombatComboManager, m_onComboSlotChanged) == 0x18, "signals must start at 0x18");
static_assert(offsetof(C_CombatComboManager, m_activeCombos) == 0xC0, "m_activeCombos offset");

}  // namespace wh::combatmodule
