#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"             // DEPENDENCY (action-base template wave)
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionGuardMovementParams.h"   // DEPENDENCY (guard-side wave): shared params type

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionRiderMovement -- mounted (rider) movement action
// (KCD2 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionRiderMovement@combatmodule@wh@@  (TD 0x184AFBF30).
// primary vtable 0x183E40D58 (68 slots)  I_ActionImpl @+0x10  C_CombatActorObject @+0x58
// creation function sub_182749D5C (operator new 0x78 -> proven sizeof; this fn both allocates AND
// constructs)   base-chain ctor sub_18090CB4C.
//
// ACTUAL BASE: C_CombatActorAnimatedAction<S_CombatActorActionGuardMovementParams, 1, I_CombatActorActionPrivate>
//   (primary @+0x00; NO extra MI interface base). It does NOT parameterise its own params type -- it
//   SHARES S_CombatActorActionGuardMovementParams with the guard-side GuardMovement action (so there is
//   no S_CombatActorActionRiderMovementParams). m_params (GuardMovementParams, 0x08) at +0x50;
//   C_CombatActorObject at +0x58. The object ends exactly at the C_CombatActorObject base -> NO leaf data.
//
// The ctor sets priority = 1 (sub_18090D1B0(this,1)) and m_actionSequenceId (+0x3C, a base field) = 1.
// [vs KCD1] 0x100 -> 0x78. KCD1's single own field `m_isRider` (init 1) is GONE in KCD2: this leaf adds
//   no data; the rider distinction is carried by the class type itself (the +0x3C=1 base write above).
//
// Own primary-vtable overrides (leaf 0x183E40D58 vs base 0x183A60988): GetActionTypeId[14]=0x181A7D850,
// combat slots [26..55] (incl. [27]=0x180939ED0), OnStart[57]=0x182755880, OnStop[58]=0x182755B74,
// OnTick[60]=0x180738C1C, GetNameString[64]=0x18275860C. Vtable not re-enumerated here.

namespace wh::combatmodule {

class C_CombatActorActionRiderMovement
    : public C_CombatActorAnimatedAction<S_CombatActorActionGuardMovementParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionRiderMovement;
    // No own data members (object ends at the C_CombatActorObject base; see header note).
};
static_assert(sizeof(C_CombatActorActionRiderMovement) == 0x78);

}  // namespace wh::combatmodule
