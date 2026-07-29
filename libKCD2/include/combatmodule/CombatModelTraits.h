#pragma once
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::combatmodule::traits -- combat-side policies of the C_ModelProperty family (kd7u).
// -----------------------------------------------
// Signal policies choose what the property's embedded C_Signal broadcasts on change. The delegate
// always receives the owning actor first (same convention as KCD1's raw S_CombatActorState
// signals): (I_CombatActor& owner, ...). Trace / ownership policies are empty tags;
// C_CombatActorModelOwnership supplies the owner-pointer type stored in each property (a
// polymorphic combat-actor pointer; virtual call at vtable+712 observed on it).

namespace wh::combatmodule { class I_CombatActor; }

namespace wh::combatmodule::traits {

// Broadcasts (owner, NEW value [, extra...]) -- e.g. <bool, I_CombatActor&>.
template<class T, class... Extra>
struct C_CombatSignalWithNewValueTrait {
    using Signal = wh::shared::C_Signal<I_CombatActor&, T, Extra...>;
};

// Broadcasts (owner, old value, new value) -- state-id transitions.
template<class T>
struct C_CombatSignalWithOldValueTrait {
    using Signal = wh::shared::C_Signal<I_CombatActor&, T, T>;
};

// Array properties: (owner, element index, new value). They embed TWO such signals
// (whole-array + per-element change, split tentative).
template<class T, class IndexEnum>
struct C_CombatSignalArrayTrait {
    using Signal = wh::shared::C_Signal<I_CombatActor&, IndexEnum, T>;
};

// Set properties: (owner, element, added/removed).
template<class T>
struct C_CombatSignalSetTrait {
    using Signal = wh::shared::C_Signal<I_CombatActor&, const T&, bool>;
};

// No debug tracing; empty tag.
struct C_CombatModelNoTrace {};

// Owner = the combat actor the property belongs to.
struct C_CombatActorModelOwnership {
    using OwnerPtr = wh::combatmodule::I_CombatActor*;
};

}  // namespace wh::combatmodule::traits
