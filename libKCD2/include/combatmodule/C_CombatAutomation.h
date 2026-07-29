#pragma once
#include <cstdint>
#include <cstddef>
#include "C_CombatActorUpdatedObject.h"
#include "I_CombatAutomation.h"
#include "S_CombatAutomationModels.h"
#include "C_CombatAutomationDirector.h"
#include "C_CombatTemporaryAutomationGuardStarter.h"
#include "C_CombatAutomationDefense.h"
#include "C_CombatAutomationAttack.h"
#include "C_CombatAutomationRiposte.h"
#include "C_CombatAutomationCombo.h"
#include "C_CombatAutomationZoneChange.h"
#include "C_CombatAutomationGuard.h"
#include "C_CombatAutomationWeapons.h"
#include "C_CombatAutomationMissile.h"
#include "C_CombatAutomationBattlement.h"
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomation -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x690.
// -----------------------------------------------
// The NPC combat "brain". ctor sub_1810EF2DC. Stored LAZILY at C_CombatActor+0x248 (created on
// first use, e.g. by the GuardStarter's update). MI: C_CombatActorUpdatedObject (primary,
// updateFlags = 9 -> phases 1+4; vtable 0x183B0A480; GetSubsystemId -> 25, GetName ->
// "Automation") + I_CombatAutomation (secondary vptr @+0x18). Eleven behaviour sub-objects are
// embedded by value; the shared data models live in the heap block m_pModels.
//
// [FUNDAMENTAL vs KCD1] The per-signal/per-model data moved into the heap model block (+0x48);
// the melee sub-behaviours were re-tiled (see each behaviour header for its own KCD1 delta).
// The two top-level signal arg lists are UNVERIFIED (KCD1-correlated: m_onAutomationSignal was
// C_Signal<I_CombatActor&, I_CombatActor&, E_SignalSource::Type>; m_onEvent a single-enum event).

namespace wh::combatmodule {

class C_CombatAutomation
    : public C_CombatActorUpdatedObject   // +0x00  (0x18; updateFlags = 9)
    , public I_CombatAutomation           // +0x18  (vptr only)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomation;
    E_CombatSubsystem GetSubsystemId() const override { return static_cast<E_CombatSubsystem>(25); }  // 0x181A7E2D0
    const char* GetName() const override { return "Automation"; }                                     // 0x181A7E150

    wh::shared::C_Signal<> m_onAutomationSignal;   // +0x020  (sentinel unk_185666898; listener wired in GetOrCreate; args unverified)
    wh::shared::C_Signal<> m_onEvent;              // +0x030  (sentinel unk_1855D4D50; args unverified)
    uint16_t m_flags40;                            // +0x040  (ctor 0)
    uint8_t  _pad42[6];                            // +0x042
    S_CombatAutomationModels* m_pModels;           // +0x048  heap 0x1B0 (built by sub_1810EEA54)
    C_CombatAutomationDirector m_director;         // +0x050  (0x90)
    C_CombatTemporaryAutomationGuardStarter m_guardStarter;   // +0x0E0  (0x38)
    C_CombatAutomationDefense  m_defense;          // +0x118  (0x50)
    C_CombatAutomationAttack   m_attack;           // +0x168  (0xB0)
    C_CombatAutomationRiposte  m_riposte;          // +0x218  (0x48)
    C_CombatAutomationCombo    m_combo;            // +0x260  (0x78)
    C_CombatAutomationZoneChange m_zoneChange;     // +0x2D8  (0x1F0)
    C_CombatAutomationGuard    m_guard;            // +0x4C8  (0x98)
    C_CombatAutomationWeapons  m_weapons;          // +0x560  (0x90)
    C_CombatAutomationMissile  m_missile;          // +0x5F0  (0x60)
    C_CombatAutomationBattlement m_battlement;     // +0x650  (0x40)
};
static_assert(sizeof(C_CombatAutomation) == 0x690, "C_CombatAutomation must be 0x690");
static_assert(offsetof(C_CombatAutomation, m_director) == 0x50, "m_director offset");
static_assert(offsetof(C_CombatAutomation, m_defense) == 0x118, "m_defense offset");
static_assert(offsetof(C_CombatAutomation, m_zoneChange) == 0x2D8, "m_zoneChange offset");
static_assert(offsetof(C_CombatAutomation, m_battlement) == 0x650, "m_battlement offset");

}  // namespace wh::combatmodule
