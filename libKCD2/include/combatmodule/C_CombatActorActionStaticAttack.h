#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"   // DEPENDENCY (action-base template wave)
#include "I_CombatActorActionPrivate.h"
#include "I_CombatActionHelperAttackOwner.h"
#include "S_CombatActorActionStaticAttackParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionStaticAttack -- stationary/precomputed attack action
// (KCD2 1.5.6, kd7u).  sizeof 0x90.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionStaticAttack@combatmodule@wh@@  (TD 0x184AFD700).
// primary vtable 0x183E40AB8 (68 slots)  I_ActionImpl @+0x10  C_CombatActorObject @+0x60  I_CombatActionHelperAttackOwner @+0x80
// object ctor sub_182753D18   factory sub_180D4AEB0 (operator new 0x90 -> proven sizeof)   base-chain ctor sub_182753BC8
//
// Inheritance: C_CombatActorAnimatedAction<S_CombatActorActionStaticAttackParams, 1, I_CombatActorActionPrivate>
//   (primary @+0x00) + I_CombatActionHelperAttackOwner (MI base @+0x80). m_params (StaticAttackParams,
//   0x10) at +0x50; C_CombatActorObject at +0x60 (params is 0x10, not 0x18 -> everything shifts -0x08
//   vs Attack). The factory selects an attack candidate and stores it into m_params.m_pAttackData (+0x58).
//
// [vs KCD1] 0x110 -> 0x90. Only own field is the attack helper (KCD1 had the same single m_pAttackHelper).
//
// Own primary-vtable overrides (leaf 0x183E40AB8 vs base 0x183A60988): GetActionTypeId[14]=0x181A72500,
// combat slots [26..55], _vfOwn0[56]=0x181704A90, OnStart[57]=0x182755948, OnStop[58]=0x182755C44,
// OnTick[60]=0x182758C18, GetNameString[64]=0x18275860C. Plus 8 HelperAttackOwner impls. Not re-enumerated.

namespace wh::combatmodule {

class C_CombatActionHelperAttack;   // owned attack helper (alloc 0x50; ctor sub_180915088; module-core wave)

class C_CombatActorActionStaticAttack
    : public C_CombatActorAnimatedAction<S_CombatActorActionStaticAttackParams, 1, I_CombatActorActionPrivate>,
      public I_CombatActionHelperAttackOwner
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionStaticAttack;
    C_CombatActionHelperAttack* m_pAttackHelper;  // +0x88  (alloc 0x50; owner = this HelperAttackOwner subobject @+0x80)
};
static_assert(sizeof(C_CombatActorActionStaticAttack) == 0x90);

}  // namespace wh::combatmodule
