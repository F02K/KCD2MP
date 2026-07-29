#pragma once
#include <cstdint>
#include "C_CombatActorUpdatedObject.h"
#include "../CryEngine/CryCommon/TimeValue.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatTemporaryAutomationGuardStarter -- KCD2 1.5.6 (kd7u).  sizeof 0x38.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0xE0 (ctor sub_1810EF298). MI: C_CombatActorUpdatedObject
// (primary, updateFlags = 4 -> phase-3 tick; primary vtable 0x183E420B0, Update = sub_181412ED0)
// + a 2-slot property/event observer whose vptr sits at +0x18 (subscribes to combat events
// 0x8D/15/17; exact base identity unresolved -- the local interface below is a layout stand-in).
// The update fires when m_activateAt is reached OR m_bPending is set: it lazily spawns the full
// C_CombatAutomation on the owner, starts a guard, then clears the request/timer/pending state.

namespace wh::combatmodule {

// Layout stand-in for the unidentified 2-slot observer base ([0] = property-change callback that
// sets m_bPending, [1] = nullsub). NOT an attested class name.
class I_GuardStarterObserver {
public:
    virtual void OnPropertyChanged() = 0;   // [0] sub_181E3F160: m_bPending = !CanStartGuard()
    virtual void _vf1() = 0;                // [1] nullsub
};

class C_CombatTemporaryAutomationGuardStarter
    : public C_CombatActorUpdatedObject   // +0x00  (0x18; updateFlags = 4)
    , public I_GuardStarterObserver       // +0x18  (vptr only)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatTemporaryAutomationGuardStarter;
    E_CombatSubsystem GetSubsystemId() const override { return static_cast<E_CombatSubsystem>(48); }  // [2] 0x181A7E2E0
    const char* GetName() const override { return ""; }   // [3] shared type-id -> name lookup impl (sub_18275DA28)
    void OnPropertyChanged() override {}
    void _vf1() override {}

    int32_t    m_request;               // +0x20  guard-start request (!=0 -> fire; semantics medium confidence)
    uint8_t    _pad24[4];               // +0x24
    CTimeValue m_activateAt;            // +0x28  init -100000; fire when now >= this
    bool       m_bListenerRegistered;   // +0x30  event subscription state (events 0x8D/15/17)
    bool       m_bPending;              // +0x31  = !CanStartGuard (set by the observer callback)
    uint8_t    _pad32[6];               // +0x32
};
static_assert(sizeof(C_CombatTemporaryAutomationGuardStarter) == 0x38,
              "C_CombatTemporaryAutomationGuardStarter must be 0x38");

}  // namespace wh::combatmodule
