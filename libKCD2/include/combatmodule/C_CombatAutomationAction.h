#pragma once
#include <cstdint>
#include "C_CombatActorUpdatedObject.h"
#include "E_CombatAutomationActionKind.h"
#include "../CryEngine/CryCommon/TimeValue.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationAction : C_CombatActorUpdatedObject -- KCD2 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// Base of every C_CombatAutomation behaviour (Director/Attack/Combo/ZoneChange/Guard/Weapons/
// Missile/Battlement/Defense/Riposte/GuardStarter). ctor sub_180C5785C(this, actor, updateFlags);
// vtable 0x183A8A6B8 (14 slots: 6 Object + 4 phase ticks + [10] GetActionKind (pure, per-leaf id) +
// [11] bool=false + 2 stubs). GetSubsystemId returns 26 for the whole family.
// All -100000 timing anchors in this family are CTimeValue int64 sentinels ("never"; reader
// sub_18094BC8C: <= -100000 -> -1.0 else (t - now) * 1e-5), NOT floats.

namespace wh::combatmodule {

class C_CombatAutomationAction : public C_CombatActorUpdatedObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationAction;
    E_CombatSubsystem GetSubsystemId() const override { return static_cast<E_CombatSubsystem>(26); }  // [2] shared family id

    virtual E_CombatAutomationActionKind GetActionKind() const = 0;  // [10]  per-leaf constant
    virtual bool _vf11() { return false; }         // [11]  default-false predicate (role unresolved)
    virtual void _vf12() {}                        // [12]
    virtual void _vf13() {}                        // [13]

    int32_t    m_stateFlags;    // +0x18  (ctor 0; role unresolved)
    uint32_t   _pad1C;          // +0x1C
    CTimeValue m_lastTime;      // +0x20  init -100000 ("never")
    int64_t    m_field28;       // +0x28  (ctor 0; init 0 is a VALID time, so CTimeValue lean weak -- unresolved)
};
static_assert(sizeof(C_CombatAutomationAction) == 0x30, "C_CombatAutomationAction must be 0x30");

}  // namespace wh::combatmodule
