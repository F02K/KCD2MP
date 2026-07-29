#pragma once
#include <cstdint>
#include "S_CombatActorActionPerfectBlockBaseParams.h"
#include "I_CombatActionBlockHelperParams.h"
#include "I_CombatActionAttackHelperParams.h"   // DEPENDENCY: attack-side wave (I_CombatActionAttackHelperParams)

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionSyncPerfectBlockParams -- params of
// C_CombatActorActionSyncPerfectBlock (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x30.  MI polymorphic.
// -----------------------------------------------
// RTTI: .?AUS_CombatActorActionSyncPerfectBlockParams@combatmodule@wh@@  (struct; TD 0x184AF90A0, COL
// 0x184160F88, vtables 0x183B928D0 / 0x183B92998 / 0x183B92A30).
// RTTI BaseClassArray is IDENTICAL in shape to S_CombatActorActionPerfectBlockParams (numBaseClasses=4):
//   S_CombatActorActionPerfectBlockBaseParams @ +0x00,  I_CombatActionBlockHelperParams @ +0x10,
//   I_CombatActionAttackHelperParams @ +0x18.
// Embedded by value at +0x50 of the SyncPerfectBlock action; C_CombatActorObject begins at +0x80 ->
// 0x30 span.  NOTE: the AttackHelperParams subobject is 0x10 (vtable + m_pAttackData), so params+0x20
// == its m_pAttackData, NOT own payload.  +0x28 payload UNVERIFIED (not written by the params ctor).

namespace wh::combatmodule {

struct S_CombatActorActionSyncPerfectBlockParams
    : public S_CombatActorActionPerfectBlockBaseParams,   // +0x00  (0x10)
      public I_CombatActionBlockHelperParams,             // +0x10  (0x08, vtable-only)
      public I_CombatActionAttackHelperParams             // +0x18  (0x10: vtable + m_pAttackData @+0x20)
{
    inline static constexpr auto RTTI = Offsets::RTTI_S_CombatActorActionSyncPerfectBlockParams;
    uint64_t m_field28;      // +0x28  zero-initialized by ctor helper sub_181A73BA0 (adjacent to m_pAttackData@+0x20); fill-later payload, write path/type unrecovered
};
static_assert(sizeof(S_CombatActorActionSyncPerfectBlockParams) == 0x30,
              "S_CombatActorActionSyncPerfectBlockParams = 3 poly subobjects (0x28) + 0x08 payload");

}  // namespace wh::combatmodule
