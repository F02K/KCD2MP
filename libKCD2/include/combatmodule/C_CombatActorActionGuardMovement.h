#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionGuardMovementParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionGuardMovement -- guard-movement action leaf (KCD2 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionGuardMovement@combatmodule@wh@@ (TD 0x184AFCCD0).
// ctor sub_18090CA88 (alloc 0x78 == PROVEN sizeof); base+anim ctor sub_18090CB4C.
// Vtables: +0x00 0x183A5F5E8 | +0x10 0x183A5FF58 (I_ActionImpl) | +0x58 0x183A5FF20 (C_CombatActorObject).
//
// TRUE Warhorse hierarchy (RTTI BaseClassArray of TD 0x184AFCCD0):
//   C_CombatActorActionGuardMovement
//     : C_CombatActorAnimatedAction<S_CombatActorActionGuardMovementParams, 1, I_CombatActorActionPrivate>
//         : C_CombatActorParamAction<S_CombatActorActionGuardMovementParams, I_CombatActorActionPrivate>
//             : C_CombatActorAction<I_CombatActorActionPrivate, C_ParamAction<..., C_Action<...>>>
//                 : C_ParamAction<S_CombatActorActionGuardMovementParams, C_Action<I_CombatActorActionPrivate>> (mdisp 0x00)
//                 : C_CombatActorObject                                                                         (mdisp 0x58)
// The extra C_CombatActorAnimatedAction<TParams, N=1, TPrivate> layer (vs. plain C_CombatActorActionGuard)
// contributes exactly ONE 8-byte animation slot at +0x70 (template count N == the "1" argument passed to
// the array-init helper sub_18050B734 by anim ctor sub_18090D378). As for the plain Guard leaf, the wrapper
// templates live in C_CombatActorAnimatedAction.h / C_CombatActorParamAction.h / C_CombatActorAction.h.
// GuardMovement adds NO own leaf data -- the object ends right after the anim slot (0x78).
//
// [vs KCD1] KCD1 was 0xF8 (Animated over the 0xD0 C_Action). KCD2 0x78.

namespace wh::combatmodule {

class C_CombatActorActionGuardMovement
    : public C_CombatActorAnimatedAction<S_CombatActorActionGuardMovementParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionGuardMovement;
};
static_assert(sizeof(C_CombatActorActionGuardMovement) == 0x78, "ctor sub_18090CA88 allocates 0x78");

}  // namespace wh::combatmodule
