#pragma once
#include <cstdint>
#include "C_Signal.h"

// -----------------------------------------------
// wh::shared::C_ModelContextProperty<Ctx,T,N> -- KCD2 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// A model property whose VALUE lives in an external context object (Ctx); this wrapper holds only
// the change signal + subscription bookkeeping. Built by helper sub_1810F1CDC (e.g. the
// <E_BlockModeContext, E_BlockMode::Type, 0> instance at S_CombatActorState+0x630); reset helper
// sub_180A1B3F8 clears +0x18/+0x1C and disconnects via +0x1D.

namespace wh::shared {

// Read-only view (own vtable; slot roles beyond the dtor unresolved).
template<class Ctx, class T>
class I_ReadonlyModelContextProperty {
public:
    virtual ~I_ReadonlyModelContextProperty() = default;   // [0]
};

template<class Ctx, class T, int N>
class C_ModelContextProperty : public I_ReadonlyModelContextProperty<Ctx, T> {
public:
    wh::shared::C_Signal<T> m_signal;       // +0x08  change-notify (0x10)
    int32_t                 m_ctxState;     // +0x18  subscription bookkeeping (semantics inferred)
    bool                    m_flag0;        // +0x1C
    bool                    m_subscribed;   // +0x1D  if set, reset disconnects m_signal
    uint8_t                 _pad1E[2];      // +0x1E
};

}  // namespace wh::shared
