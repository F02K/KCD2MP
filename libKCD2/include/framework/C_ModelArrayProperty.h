#pragma once
#include <cstdint>
#include "C_Signal.h"
#include "ModelPropertyTraits.h"
#include "../combatmodule/CombatModelTraits.h"

// -----------------------------------------------
// wh::shared::C_ModelArrayProperty<T,IndexEnum,N,Def,SigArr,Trace,Own> -- KCD2 1.5.6 (kd7u).
// -----------------------------------------------
// A model property holding T[N] indexed by an enum, with TWO change signals (whole-array +
// per-element, split tentative). Live S_CombatActorState instantiations (strides proven from
// consecutive ctor offsets):
//   <bool,  E_GuardRequestScope::Type, 8, Standard, ...>  = 0x48  (ctor a1+2904)
//   <int,   E_HandSlot,                2, Custom,   ...>  = 0x48  (ctor sub_1810F0894)
//   <WUID,  E_HandSlot,                2, Custom,   ...>  = 0x50  (ctor sub_1810F0064)
// The 8-byte default slot exists in all variants (unused when Standard).

namespace wh::shared {

// Read-only view (own vtable; slot roles beyond the dtor unresolved).
template<class T, class IndexEnum, int N, class SigArr, class Trace>
class I_ReadonlyModelArrayProperty {
public:
    virtual ~I_ReadonlyModelArrayProperty() = default;   // [0]
};

template<class T, class IndexEnum, int N,
         class Def    = traits::C_StandardDefaultValueTrait<T>,
         class SigArr = wh::combatmodule::traits::C_CombatSignalArrayTrait<T, IndexEnum>,
         class Trace  = wh::combatmodule::traits::C_CombatModelNoTrace,
         class Own    = wh::combatmodule::traits::C_CombatActorModelOwnership>
class C_ModelArrayProperty : public I_ReadonlyModelArrayProperty<T, IndexEnum, N, SigArr, Trace> {
public:
    T                        m_values[N];      // +0x08  element storage
    T                        m_default;        // 8-byte slot after values (meaningful for Custom only)
    typename Own::OwnerPtr   m_pOwner;         //        owning combat actor
    typename SigArr::Signal  m_signalArray;    //        whole-array change (0x10)
    typename SigArr::Signal  m_signalElement;  //        per-element change (0x10; split tentative)
    uint8_t                  _tail[8];         //        trailing pad (never written)
};

}  // namespace wh::shared
