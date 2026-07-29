#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionPoseModifierParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionPoseModifier -- pose-modifier action leaf (KCD2 1.5.6, kd7u).  sizeof 0x88.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionPoseModifier@combatmodule@wh@@ (TD 0x184AF9880).
// ctor sub_18090D3A0 (alloc 0x88 == PROVEN sizeof); base ctor sub_18090D1F4; dtor sub_18090CC50.
// Vtables: +0x00 0x183A60010 | +0x10 0x183A60B98 (I_ActionImpl) | +0x58 0x183A60C50 (C_CombatActorObject).
//
// TRUE Warhorse hierarchy (RTTI BaseClassArray of TD 0x184AF9880): same wrapper chain as GuardMovement,
// parameterised with S_CombatActorActionPoseModifierParams; C_CombatActorAnimatedAction<...,1,...> layer
// present (anim slot @+0x70; templates in C_CombatActorAnimatedAction.h).
// The base ctor sets the C_Action m_actionSequenceId (+0x3C, base subobject) to 4 and m_priority (+0x38)
// to 1 -- base-region initialisation, not leaf fields. Leaf own data: a bool flag @+0x78 and a float timer
// @+0x80 initialised to -100000.0f (a "long-ago"/inactive sentinel).
//
// [vs KCD1] KCD1 was 0x108 with the same {bool m_flag; float m_timer(-100000)} own layout. KCD2 0x88.

namespace wh::combatmodule {

class C_CombatActorActionPoseModifier
    : public C_CombatActorAnimatedAction<S_CombatActorActionPoseModifierParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionPoseModifier;
    // ---- C_CombatActorActionPoseModifier own data (+0x78..+0x88) ----
    bool     m_flag;               // +0x78  (init 0)
    uint8_t  _pad79[7];            // +0x79
    float    m_timer;              // +0x80  (init -100000.0f; inactive sentinel)
    uint32_t _pad84;               // +0x84
};
static_assert(sizeof(C_CombatActorActionPoseModifier) == 0x88, "ctor sub_18090D3A0 allocates 0x88");

}  // namespace wh::combatmodule
