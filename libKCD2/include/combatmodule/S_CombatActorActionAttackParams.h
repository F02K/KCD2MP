#pragma once
#include <cstdint>
#include "I_CombatActionAttackHelperParams.h"

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionAttackParams -- params embedded by C_CombatActorActionAttack
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18.
// -----------------------------------------------
// RTTI: .?AUS_CombatActorActionAttackParams@combatmodule@wh@@  (TD 0x184AFA980, vtable 0x183A61540).
// Polymorphic `struct`; single base I_CombatActionAttackHelperParams (+0x00), verified from its RTTI
// base array (numBaseClasses=2). Sits at action+0x50 (C_ParamAction::m_params).
//
// Layout proved by the Attack base ctor sub_18091581C:
//   +0x50 (obj) = &S_CombatActorActionAttackParams::vftable
//   +0x58 (obj) = 0            -> I_CombatActionAttackHelperParams::m_pAttackData (+0x08)
//   +0x60 (obj) = 0x101 (u32)  -> m_flags (+0x10)   [KCD1 had 0x100; KCD2 = 0x101]
//
// [vs KCD1] KCD1 put m_pAttackData in this struct and derived from the reflect base; KCD2 moves
// m_pAttackData up into I_CombatActionAttackHelperParams (shared by Static/Sync), so this struct adds
// only m_flags.

namespace wh::combatmodule {

struct S_CombatActorActionAttackParams : public I_CombatActionAttackHelperParams {
    inline static constexpr auto RTTI = Offsets::RTTI_S_CombatActorActionAttackParams;
    uint32_t m_flags;    // +0x10  (init 0x101)
    uint32_t _pad14;     // +0x14
};
static_assert(sizeof(S_CombatActorActionAttackParams) == 0x18);

}  // namespace wh::combatmodule
