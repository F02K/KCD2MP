#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::shared::traits -- default-value / save-load policies of the C_ModelProperty family (kd7u).
// -----------------------------------------------
// Recovered from the RTTI of the S_CombatActorState property instantiations, e.g.
//   wh::shared::C_ModelProperty< E_CombatActorStateId::Type,
//                                wh::shared::traits::C_StaticDefaultValueTrait<...,1>,
//                                wh::combatmodule::traits::C_CombatSignalWithOldValueTrait<...>,
//                                wh::combatmodule::traits::C_CombatModelNoTrace,
//                                wh::shared::traits::C_NoSaveLoad,
//                                wh::combatmodule::traits::C_CombatActorModelOwnership >
// Standard/Static store no default (empty tags); Custom stores one at property+0x2C (its
// _reserved word reproduces the 4-byte gap the ctor leaves at +0x28).

namespace wh::shared::traits {

// Default = T{} (zero); nothing stored.
template<class T>
struct C_StandardDefaultValueTrait {};

// Default = compile-time V (e.g. <int,-1>); nothing stored.
template<class T, T V>
struct C_StaticDefaultValueTrait {
    static constexpr T value = V;
};

// Default read from config/global at init; stored in the property at +0x2C.
template<class T>
struct C_CustomDefaultValueTrait {
    uint32_t _reserved;   // +0x00  (never written; keeps m_default at property+0x2C)
    T        m_default;   // +0x04
};

// No serialization; empty tag.
struct C_NoSaveLoad {};

// ---- GENERIC (non-combat) policy family, RTTI-named in the C_Player state properties ----
// Under C_OwnershipEmpty the property stores NO owner pointer: the C_Signal moves up to +0x10 and
// the stride shrinks to 0x20 (proven: GetValue -> *(u8*)(this+8), GetSignal -> this+0x10).

// Broadcasts the new value; no owner argument (generic form).
template<class T>
struct C_SignalWithNewValueTrait {};

// No debug tracing; empty tag (generic form; ToDbgStr returns the static empty string).
struct C_DebugNoTrace {};

// No stored owner; empty tag (property shrinks to 0x20).
struct C_OwnershipEmpty {};

}  // namespace wh::shared::traits
