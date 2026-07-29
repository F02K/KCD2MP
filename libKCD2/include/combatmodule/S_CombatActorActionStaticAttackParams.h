#pragma once
#include <cstdint>
#include "I_CombatActionAttackHelperParams.h"

// -----------------------------------------------
// wh::combatmodule::S_CombatActorActionStaticAttackParams -- params embedded by
// C_CombatActorActionStaticAttack (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI: .?AUS_CombatActorActionStaticAttackParams@combatmodule@wh@@  (TD 0x184AF9050, vtable 0x183E3DFB0).
// Polymorphic `struct`; single base I_CombatActionAttackHelperParams (+0x00), RTTI base array
// numBaseClasses=2. Adds NO data of its own -- it IS the base (0x10): a distinct type only because it
// carries its own deleting dtor (vtable slot [0] = 0x182754F68) and type identity.
//
// Layout proved by the StaticAttack base ctor sub_182753BC8:
//   +0x50 (obj) = &S_CombatActorActionStaticAttackParams::vftable
//   +0x58 (obj) = 0   -> m_pAttackData (+0x08)   [set to the chosen candidate by factory sub_180D4AEB0]
// C_CombatActorObject base of the action begins at obj+0x60, confirming this struct ends at 0x10.

namespace wh::combatmodule {

struct S_CombatActorActionStaticAttackParams : public I_CombatActionAttackHelperParams {
    inline static constexpr auto RTTI = Offsets::RTTI_S_CombatActorActionStaticAttackParams;
    // No own data members (exactly I_CombatActionAttackHelperParams; distinct type/dtor only).
};
static_assert(sizeof(S_CombatActorActionStaticAttackParams) == 0x10);

}  // namespace wh::combatmodule
