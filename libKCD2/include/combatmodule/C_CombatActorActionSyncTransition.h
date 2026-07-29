#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"   // DEPENDENCY (action-base template wave)
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionSyncTransitionParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionSyncTransition -- synchronized transition action
// (KCD2 1.5.6, kd7u).  sizeof 0x98.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionSyncTransition@combatmodule@wh@@  (TD 0x184AFABF0).
// primary vtable 0x183B0B2C0 (68 slots)  I_ActionImpl @+0x10  C_CombatActorObject @+0x68
// object ctor sub_1810F2C58   factory sub_1810F2BCC (operator new 0x98 -> proven sizeof)   base-chain ctor sub_1810F2DD8
//
// Inheritance: C_CombatActorAnimatedAction<S_CombatActorActionSyncTransitionParams, 1, I_CombatActorActionPrivate>
//   (primary @+0x00; NO extra MI interface base). m_params (SyncTransitionParams, 0x18) at +0x50;
//   C_CombatActorObject at +0x68; leaf data begins at +0x88. Priority set to 7 (sub_18090D1B0(this,7)).
//
// [vs KCD1] 0x110 -> 0x98. Fields map: pSyncHelper, syncFlag (same as KCD1).
//
// Own primary-vtable overrides (leaf 0x183B0B2C0 vs base 0x183A60988): GetActionTypeId[14]=0x181A7D880,
// combat slots [26..55], _vfOwn0[56]=0x181704A90, OnStart[57]=0x180D4C030, OnStop[58]=0x180D4BE04,
// OnTick[60]=0x180D4D240, GetNameString[64]=0x18275860C. Vtable not re-enumerated here.

namespace wh::combatmodule {

class C_CombatActionEarlyExitHelper;   // owned early-exit/sync helper (alloc 0x18; ctor sub_180914FA4; RTTI-named)

class C_CombatActorActionSyncTransition
    : public C_CombatActorAnimatedAction<S_CombatActorActionSyncTransitionParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionSyncTransition;
    C_CombatActionEarlyExitHelper* m_pSyncHelper;   // +0x88  (alloc 0x18)
    bool                      m_syncFlag;      // +0x90  (init 0)
    uint8_t                   _pad91[7];       // +0x91
};
static_assert(sizeof(C_CombatActorActionSyncTransition) == 0x98);

}  // namespace wh::combatmodule
