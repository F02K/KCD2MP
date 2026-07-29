#pragma once
#include <cstdint>
#include <set>
#include "C_Signal.h"
#include "../combatmodule/CombatModelTraits.h"

// -----------------------------------------------
// wh::shared::C_ModelSetProperty<T,SigSet,Trace,Own> -- KCD2 1.5.6 (kd7u).  sizeof 0x38.
// -----------------------------------------------
// A model property holding a std::set<T> instead of a scalar (ctor helper sub_1810F1D14; the
// S_CombatActorState instance holds std::set<S_RestrictedInputClass>, node payload 0x20, recursive
// MSVC tree dtor sub_18175335C).

namespace wh::shared {

// Read-only view (own vtable; slot roles beyond the dtor unresolved).
template<class T, class SigSet>
class I_ReadonlyModelSetProperty {
public:
    virtual ~I_ReadonlyModelSetProperty() = default;   // [0]
};

template<class T,
         class SigSet = wh::combatmodule::traits::C_CombatSignalSetTrait<T>,
         class Trace  = wh::combatmodule::traits::C_CombatModelNoTrace,
         class Own    = wh::combatmodule::traits::C_CombatActorModelOwnership>
class C_ModelSetProperty : public I_ReadonlyModelSetProperty<T, SigSet> {
public:
    std::set<T>              m_value;    // +0x08  the stored set (0x10)
    typename Own::OwnerPtr   m_pOwner;   // +0x18  owning combat actor
    typename SigSet::Signal  m_signal;   // +0x20  change-notify (0x10)
    uint8_t                  _pad30[8];  // +0x30  tail pad (empty trailing Trace/Own policy member + align); never ctor-init/dtor/read -- forces sizeof to 0x38 (cf. C_ModelArrayProperty::_tail)
};

}  // namespace wh::shared
