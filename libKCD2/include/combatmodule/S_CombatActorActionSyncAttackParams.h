#pragma once
#include <cstdint>
#include "I_CombatActionAttackHelperParams.h"

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionSyncAttackParams -- params embedded by
// C_CombatActorActionSyncAttack (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18.
// -----------------------------------------------
// RTTI: .?AUS_CombatActorActionSyncAttackParams@combatmodule@wh@@  (TD 0x184AF9000, vtable 0x183A8AEF0).
// Polymorphic `struct`; single base I_CombatActionAttackHelperParams (+0x00), RTTI base array
// numBaseClasses=2. Overrides most of the 23 base getters (its vtable slots [1..22] differ from the
// Attack/Static defaults).
//
// Layout proved by the SyncAttack base ctor sub_180C621A0:
//   +0x50 (obj) = &S_CombatActorActionSyncAttackParams::vftable
//   +0x58 (obj) = 0            -> m_pAttackData (+0x08)
//   +0x60 (obj) = 0 (qword)    -> m_field10 (+0x10; 8 bytes zeroed; role UNVERIFIED)
// C_CombatActorObject base of the action begins at obj+0x68, confirming this struct ends at 0x18.

namespace wh::combatmodule {

struct S_CombatActorActionSyncAttackParams : public I_CombatActionAttackHelperParams {
    inline static constexpr auto RTTI = Offsets::RTTI_S_CombatActorActionSyncAttackParams;
    uint64_t m_field10;   // +0x10  init 0 (ctor sub_180C621A0@0x180C621EB); NOT read by any of the 22 SyncAttack getters (all deref m_pAttackData@+0x08); creator sub_180C61FA8 never writes it. Role UNVERIFIED.
};
static_assert(sizeof(S_CombatActorActionSyncAttackParams) == 0x18);

}  // namespace wh::combatmodule
