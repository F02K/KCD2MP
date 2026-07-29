#pragma once
#include <cstdint>
#include "C_CombatActorUpdatedObject.h"
#include "../CryEngine/CryCommon/smartptr.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorOpponentManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40.
// -----------------------------------------------
// ctor sub_180917A64. Stored at C_CombatActor+0x3B8. Chain: C_CombatActorUpdatedObject
// (updateSubsystemIndex = 4) : C_CombatActorObject. The ctor subscribes the target-swap handler
// (sub_180912370) to the owner's current-target-changed dispatch (owner vtable slot +0x368).
//
// [FUNDAMENTAL vs KCD1] This manager now owns the opponent locking that KCD1 kept partly on
// S_CombatActorState -- KCD1-style reads of state->m_pOpponent must go through this object instead.

namespace wh::combatmodule {

class I_CombatActorAction;

// One opponent action-lock slot (0x10). Slot[0] = pending/candidate; slot[1] = active exclusive
// lock (roles inferred from the handler's writers, offsets exact).
struct S_ActionLock {
    _smart_ptr<I_CombatActorAction> m_pAction;   // +0x00
    bool    m_flag;                              // +0x08  exclusivity/request flag
    uint8_t _pad09[7];                           // +0x09
};
static_assert(sizeof(S_ActionLock) == 0x10, "S_ActionLock must be 0x10");

class C_CombatActorOpponentManager : public C_CombatActorUpdatedObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorOpponentManager;
    S_ActionLock m_lockActions[2];   // +0x18  [0] pending, [1] active exclusive lock
    int32_t  m_suppressCounter;      // +0x38  ++ when a new opponent lock is taken
    uint32_t _pad3C;                 // +0x3C
};
static_assert(sizeof(C_CombatActorOpponentManager) == 0x40, "C_CombatActorOpponentManager must be 0x40");

}  // namespace wh::combatmodule
