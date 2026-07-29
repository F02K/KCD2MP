#pragma once
#include "C_Action.h"

// -----------------------------------------------
// wh::framework::C_ParamAction<TParams, TBase> -- action + embedded params (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Extends TBase (always a C_Action<TPrivate> instantiation) with the action's parameter block
// embedded BY VALUE at offset sizeof(TBase). For the combat actions TBase = C_Action<...> (0x50),
// so m_params sits at +0x50 -- proved by the C_CombatActorActionAttack ctor sub_18091581C, which
// after calling the C_Action base ctor writes &S_CombatActorActionAttackParams::vftable to +0x50.
//
// C_ParamAction adds NO primary virtuals of its own; it re-uses TBase's vtable layout (per-type
// overrides only). TParams may be polymorphic (its own vtable at +0x50) or POD:
//   - Attack family params derive from I_CombatActionAttackHelperParams (own vtable).
//   - Block  family params derive from I_CombatActionBlockHelperParams  (own vtable).
//   - other params (Guard/Dodge/Hit/PoseModifier/...) are POD value structs (no vtable/TD).
//
// [vs KCD1] Same shape; only the offset moved (KCD1 C_Action was 0xD0, so m_params was at +0xD0).
// [vs KCD1] The unified reflection base I_CombatActorActionParamsReflect (KCD1, 25-slot vtable that
// every polymorphic params inherited) is GONE in KCD2 -- replaced by the per-family helper-params
// interfaces above (belong to a later RE wave).

namespace wh::framework {

template<typename TParams, typename TBase>
class C_ParamAction : public TBase {
public:
    TParams m_params;   // +sizeof(TBase)  (== +0x50 when TBase = C_Action<...>)
};

}  // namespace wh::framework
