#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionBlockTriggerParams -- params of C_CombatActorActionBlockTrigger
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.  PLAIN POD (no RTTI TypeDescriptor / no vtable).
// -----------------------------------------------
// Not polymorphic: the BlockTrigger action does NOT derive its params from I_CombatActionBlockHelperParams
// (only Block / PerfectBlock / SyncPerfectBlock do).  Embedded by value at +0x50 of the action; the
// C_CombatActorObject base begins at +0x58, proving the params block is exactly 8 bytes.
//
// Layout from the action base ctor sub_18169965C: writes a WORD 1 to params+0x00 (only field touched).
// Field semantics UNVERIFIED (a small flag/kind initialised to 1).

namespace wh::combatmodule {

struct S_CombatActorActionBlockTriggerParams {
    uint16_t m_field00;      // +0x00  WORD 1 written by action base ctor sub_18169965C (0x181699694); only field touched; params embedded in C_ParamAction<S_CombatActorActionBlockTriggerParams,...> [role UNVERIFIED]
    uint8_t  _pad02[6];      // +0x02
};
static_assert(sizeof(S_CombatActorActionBlockTriggerParams) == 0x08,
              "S_CombatActorActionBlockTriggerParams POD, 8 bytes");

}  // namespace wh::combatmodule
