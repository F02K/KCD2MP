#pragma once
#include "I_CombatActorAction.h"

// -----------------------------------------------
// wh::combatmodule::I_CombatActorActionPrivate -- private combat action interface (KCD2 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI: .?AVI_CombatActorActionPrivate@combatmodule@wh@@  (TD 0x184AFE170; abstract -- no vtable).
// This is the template argument that the concrete actions parameterise C_Action with:
// C_Action<I_CombatActorActionPrivate> (see the C_ParamAction<...> RTTI BaseClassArray, which lists
//   C_ParamAction -> C_Action -> I_CombatActorActionPrivate -> I_CombatActorAction -> I_Action
//   -> _i_multithread_reference_target -> I_ActionImpl).
//
// No additional primary vtable slots could be attributed to this layer specifically: the 30 combat
// slots [26..55] are declared on I_CombatActorAction (the exact I_CombatActorAction | Private split
// is not statically recoverable from the abstract bases; see I_CombatActorAction.h). Declaring zero
// new virtuals here keeps the C_Action<I_CombatActorActionPrivate> vtable at its verified 65 slots.

namespace wh::combatmodule {

class I_CombatActorActionPrivate : public I_CombatActorAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActorActionPrivate;
    // No new virtuals or data attributed to this layer (see header note).
};
static_assert(sizeof(I_CombatActorActionPrivate) == 0x10, "I_CombatActorActionPrivate = I_Action (vtable + refcount)");

}  // namespace wh::combatmodule
