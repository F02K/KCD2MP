#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"   // DEPENDENCY (action-base template wave)
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionFailedAttackParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionFailedAttack -- failed/aborted attack action
// (KCD2 1.5.6, kd7u).  sizeof 0x78.  [KCD2-NEW: no KCD1 reference]
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionFailedAttack@combatmodule@wh@@  (TD 0x184AFADB0).
// primary vtable 0x1846F61A0 (68 slots)  I_ActionImpl @+0x10  C_CombatActorObject @+0x58
// creation function sub_182749BA0 (operator new 0x78 -> proven sizeof; allocates AND constructs)
// base-chain ctor sub_182753B34.
//
// Inheritance: C_CombatActorAnimatedAction<S_CombatActorActionFailedAttackParams, 1, I_CombatActorActionPrivate>
//   (primary @+0x00; NO extra MI interface base -- unlike the real attack leaves it has NO
//   I_CombatActionHelperAttackOwner and NO attack helper). m_params (FailedAttackParams POD, 0x08) at
//   +0x50; C_CombatActorObject at +0x58. Object ends at the C_CombatActorObject base -> NO leaf data.
//
// The ctor sets priority = 1 (sub_18090D1B0(this,1)) and m_actionSequenceId (+0x3C, a base field) = 4.
//
// Own primary-vtable overrides (leaf 0x1846F61A0 vs base 0x183A60988): GetActionTypeId[14]=0x181A72890,
// combat slots [26..55] (incl. [27]=0x180939ED0), OnStart[57]=0x182755610, OnStop[58]=0x1803B6E80
// (nullsub -- empty stop), OnTick[60]=0x182758B14, GetNameString[64]=0x18275860C. Not re-enumerated.

namespace wh::combatmodule {

class C_CombatActorActionFailedAttack
    : public C_CombatActorAnimatedAction<S_CombatActorActionFailedAttackParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionFailedAttack;
    // No own data members (object ends at the C_CombatActorObject base; see header note).
};
static_assert(sizeof(C_CombatActorActionFailedAttack) == 0x78);

}  // namespace wh::combatmodule
