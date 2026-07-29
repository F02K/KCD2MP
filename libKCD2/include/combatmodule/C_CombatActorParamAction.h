#pragma once
#include "C_CombatActorAction.h"
#include "../framework/C_Action.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorParamAction<TParams, TPrivate> -- convenience combiner
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Second of the three intermediate templates that replaced KCD1's C_CombatActorActionT. Binds the
// framework C_ParamAction/C_Action pair into the combat wrapper:
//   C_CombatActorParamAction<TParams,TPrivate>
//     : C_CombatActorAction<TPrivate, framework::C_ParamAction<TParams, framework::C_Action<TPrivate>>>
// Adds NO data of its own (proven: every leaf's RTTI base array shows it contributing only itself,
// numContainedBases == its parent's + 1, and no ctor writes between the C_CombatActorAction region
// and the leaf/AnimatedAction region). Whether one of the two combat-wrapper vtable slots [65]/[66]
// is declared here rather than in C_CombatActorAction is UNVERIFIED (adjacent either way -- final
// slot order is identical).
//
// Directly-derived leaves (no Animated layer): C_CombatActorActionGuard (the only guard leaf
// without it), C_CombatActorActionBlockTrigger, C_CombatActorActionFreeBlock.

namespace wh::combatmodule {

template<typename TParams, typename TPrivate>
class C_CombatActorParamAction
    : public C_CombatActorAction<TPrivate,
                                 wh::framework::C_ParamAction<TParams, wh::framework::C_Action<TPrivate>>> {
};

}  // namespace wh::combatmodule
