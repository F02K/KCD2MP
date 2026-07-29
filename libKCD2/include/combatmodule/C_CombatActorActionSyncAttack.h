#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"   // DEPENDENCY (action-base template wave)
#include "I_CombatActorActionPrivate.h"
#include "I_CombatActionHelperAttackOwner.h"
#include "S_CombatActorActionSyncAttackParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionSyncAttack -- synchronized (paired) attack action
// (KCD2 1.5.6, kd7u).  sizeof 0xB0.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionSyncAttack@combatmodule@wh@@  (TD 0x184AFB510).
// primary vtable 0x183A8B608 (68 slots)  I_ActionImpl @+0x10  C_CombatActorObject @+0x68  I_CombatActionHelperAttackOwner @+0x88
// object ctor sub_180C61FA8   factory sub_180C61F1C (operator new 0xB0 -> proven sizeof)   base-chain ctor sub_180C621A0
//
// Inheritance: C_CombatActorAnimatedAction<S_CombatActorActionSyncAttackParams, 1, I_CombatActorActionPrivate>
//   (primary @+0x00) + I_CombatActionHelperAttackOwner (MI base @+0x88). m_params (SyncAttackParams,
//   0x18) at +0x50; C_CombatActorObject at +0x68.
//
// [vs KCD1] 0x128 -> 0xB0. KCD1 fields map: pAttackHelper->+0x90, pSyncHelper->+0x98, (new qword @+0xA0),
//   syncFlag->+0xA8.
//
// Own primary-vtable overrides (leaf 0x183A8B608 vs base 0x183A60988): GetActionTypeId[14]=0x181A7D860,
// combat slots [26..55], _vfOwn0[56]=0x1816A9DE8, OnStart[57]=0x180D4B794, OnStop[58]=0x180D4BA30,
// OnTick[60]=0x180D4CEC0, GetNameString[64]=0x18275860C. Plus 8 HelperAttackOwner impls. Not re-enumerated.

namespace wh::combatmodule {

class C_CombatActionHelperAttack;   // owned attack helper (alloc 0x50; ctor sub_180915088; module-core wave)
class C_CombatActionEarlyExitHelper;   // owned early-exit/sync helper (alloc 0x18; ctor sub_180914FA4; RTTI-named)

class C_CombatActorActionSyncAttack
    : public C_CombatActorAnimatedAction<S_CombatActorActionSyncAttackParams, 1, I_CombatActorActionPrivate>,
      public I_CombatActionHelperAttackOwner
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionSyncAttack;
    C_CombatActionHelperAttack* m_pAttackHelper;  // +0x90  (alloc 0x50; owner = this HelperAttackOwner subobject @+0x88)
    C_CombatActionEarlyExitHelper* m_pSyncHelper;    // +0x98  (alloc 0x18)
    void*                       m_pSyncPartner;   // +0xA0  owning _smart_ptr back-ref to the paired sync action (pointee class unresolved); OnStop sub_180D4BA30 SetPriority(2) @0x180D4BA59, zeroes pointee+0xC0 @0x180D4BA61, then releases null+slot2 via sub_180C5385C @0x180D4BA6C -- identical +0xA0 back-ref mechanism documented in C_CombatActorActionSyncHit / C_CombatActorActionSyncPerfectBlock
    bool                        m_syncFlag;       // +0xA8  (init 0)
    uint8_t                     _padA9[7];        // +0xA9
};
static_assert(sizeof(C_CombatActorActionSyncAttack) == 0xB0);

}  // namespace wh::combatmodule
