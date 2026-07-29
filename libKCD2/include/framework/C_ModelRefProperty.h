#pragma once
#include "C_ModelProperty.h"

// -----------------------------------------------
// wh::shared::C_ModelRefProperty<Sig,Trace,Own> -- KCD2 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// IS-A C_ModelProperty<int> (RTTI base chain: C_ModelRefProperty -> C_ModelProperty<int,Standard,
// Sig,Trace,C_NoSaveLoad,Own> -> I_ReadonlyModelProperty<int,Sig>). Adds NO data; only overrides
// the vtable to reinterpret the stored int as an entity reference/handle. 8 instances live inside
// S_CombatActorState (ctor path 0x1810f1621 etc.).

namespace wh::shared {

template<class Sig   = wh::combatmodule::traits::C_CombatSignalWithNewValueTrait<int>,
         class Trace = wh::combatmodule::traits::C_CombatModelNoTrace,
         class Own   = wh::combatmodule::traits::C_CombatActorModelOwnership>
class C_ModelRefProperty
    : public C_ModelProperty<int, traits::C_StandardDefaultValueTrait<int>, Sig, Trace,
                             traits::C_NoSaveLoad, Own> {
};
static_assert(sizeof(C_ModelRefProperty<>) == 0x30, "C_ModelRefProperty must be 0x30");

}  // namespace wh::shared
