#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionGuardVarParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionGuardVar -- KCD2-NEW guard-variation action leaf (1.5.6, kd7u).  sizeof 0x80.
// -----------------------------------------------
// [KCD2-NEW] No KCD1 counterpart. Ties in with the KCD2-added C_CombatActorGuardVarSystem subsystem
// (C_CombatActor+0x3D8). RTTI: .?AVC_CombatActorActionGuardVar@combatmodule@wh@@ (TD 0x184AF9990).
// ctor sub_1810F2EBC (alloc 0x80 == PROVEN sizeof); base ctor sub_1810F2F9C. Vtables (via find_vtables.py):
//   +0x00 0x183B0BCE8 | +0x10 0x183B0B598 (I_ActionImpl) | +0x58 0x183B0B4E8 (C_CombatActorObject).
//
// TRUE Warhorse hierarchy (RTTI BaseClassArray of TD 0x184AF9990): same wrapper chain as GuardMovement,
// parameterised with S_CombatActorActionGuardVarParams; C_CombatActorAnimatedAction<...,1,...> layer present
// (anim slot @+0x70; templates in C_CombatActorAnimatedAction.h). The base/leaf ctors set the C_Action m_actionSequenceId (+0x3C, base
// subobject) to 5 and m_priority (+0x38) to 2 -- base-region init, not leaf fields. Leaf own data: a single
// bool flag @+0x78.

namespace wh::combatmodule {

class C_CombatActorActionGuardVar
    : public C_CombatActorAnimatedAction<S_CombatActorActionGuardVarParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionGuardVar;
    // ---- C_CombatActorActionGuardVar own data (+0x78..+0x80) ----
    bool     m_flag;               // +0x78  (init 0)
    uint8_t  _pad79[7];            // +0x79
};
static_assert(sizeof(C_CombatActorActionGuardVar) == 0x80, "ctor sub_1810F2EBC allocates 0x80");

}  // namespace wh::combatmodule
