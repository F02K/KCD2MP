#pragma once
#include "I_CombatActorActionPrivate.h"

// -----------------------------------------------
// wh::combatmodule::I_CombatActorActionBlock -- block action interface (KCD2 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI: .?AVI_CombatActorActionBlock@combatmodule@wh@@  (TD 0x184AFD788; abstract -- no vtable).
// Extends I_CombatActorActionPrivate with block-specific virtuals; the concrete block action
// parameterises C_Action with it: C_Action<I_CombatActorActionBlock>.
//
// Adds EXACTLY 2 primary vtable slots [56..57] (both _purecall / leaf-implemented), proved by
// diffing C_Action<I_CombatActorActionBlock> (0x183A89F48, 67 slots) against
// C_Action<I_CombatActorActionPrivate> (0x183A60988, 65 slots): slots [0..55] are byte-identical,
// Block inserts two new pure slots at [56][57], and the C_Action-own tail then follows at [58..66].
//
// [vs KCD1] KCD1 declared no methods on this layer; KCD2 has 2. Used only by the block action
// (C_CombatActorAction<I_CombatActorActionBlock, C_ParamAction<S_CombatActorActionBlockParams, ...>>);
// PerfectBlock/FreeBlock use the ...Private chain instead.

namespace wh::combatmodule {

class I_CombatActorActionBlock : public I_CombatActorActionPrivate {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActorActionBlock;
    // ---- block-specific virtuals (primary vtable slots [56..57]) ----
    virtual void _vf56() = 0;   // [56] _purecall  (leaf-implemented)
    virtual void _vf57() = 0;   // [57] _purecall  (leaf-implemented)
    // No new data members (inherits I_Action's vtable ptr + m_nRefCounter).
};
static_assert(sizeof(I_CombatActorActionBlock) == 0x10, "I_CombatActorActionBlock = I_Action (vtable + refcount)");

}  // namespace wh::combatmodule
