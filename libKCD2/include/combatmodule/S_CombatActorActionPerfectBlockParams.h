#pragma once
#include <cstdint>
#include "S_CombatActorActionPerfectBlockBaseParams.h"
#include "I_CombatActionBlockHelperParams.h"
#include "I_CombatActionAttackHelperParams.h"   // DEPENDENCY: attack-side wave (I_CombatActionAttackHelperParams)

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionPerfectBlockParams -- params of C_CombatActorActionPerfectBlock
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x30.  MULTIPLE-INHERITANCE polymorphic params.
// -----------------------------------------------
// RTTI: .?AUS_CombatActorActionPerfectBlockParams@combatmodule@wh@@  (struct; TD 0x184AF95E0, COL
// 0x184160F60, vtables 0x183BE07E0 / 0x183BE08A8 / 0x183BE0940).
// RTTI BaseClassArray (numBaseClasses=4), most-derived first:
//   self
//   S_CombatActorActionPerfectBlockBaseParams  @ +0x00   (abstract primary base; supplies +0x00 vtable)
//   I_CombatActionBlockHelperParams            @ +0x10
//   I_CombatActionAttackHelperParams           @ +0x18   (attack-side interface -- both families' reflect)
//
// This is why the PerfectBlock family carries a FATTER 0x30 params block (vs the 0x10 plain Block
// params): three polymorphic subobjects (0x10 + 0x08 + 0x10) plus 0x08 of own payload.  Embedded by
// value at +0x50 of the action; C_CombatActorObject begins at +0x80, proving the 0x30 span.
// Layout from the params-constructing ctor sub_18164F43C (writes the three vtables to +0x50/+0x60/+0x68
// of the action == params +0x00/+0x10/+0x18, and byte 0 to params+0x08).  NOTE: the AttackHelperParams
// subobject is 0x10 (vtable + m_pAttackData), so params+0x20 == its m_pAttackData (obj+0x70), NOT own
// payload.  The +0x28 payload is not touched by the params ctor (filled by the query/factory that
// builds the params) -- UNVERIFIED.

namespace wh::combatmodule {

struct S_CombatActorActionPerfectBlockParams
    : public S_CombatActorActionPerfectBlockBaseParams,   // +0x00  (0x10)
      public I_CombatActionBlockHelperParams,             // +0x10  (0x08, vtable-only)
      public I_CombatActionAttackHelperParams             // +0x18  (0x10: vtable + m_pAttackData @+0x20)
{
    inline static constexpr auto RTTI = Offsets::RTTI_S_CombatActorActionPerfectBlockParams;
    uint64_t m_field28;      // +0x28  (payload; UNVERIFIED)
};
static_assert(sizeof(S_CombatActorActionPerfectBlockParams) == 0x30,
              "S_CombatActorActionPerfectBlockParams = 3 poly subobjects (0x28) + 0x08 payload");

}  // namespace wh::combatmodule
