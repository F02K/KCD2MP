#pragma once
#include "C_CombatActorObject.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActionHelper -- abstract base of the attack/block action helpers
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10 (adds no data over C_CombatActorObject).
// -----------------------------------------------
// RTTI .?AVC_CombatActionHelper@combatmodule@wh@@ (TD 0x4AFA9C8). NEW in KCD2: it is a common
// single-inheritance layer between the helper leaves and C_CombatActorObject. It has no COL / no
// standalone vtable and is never instantiated -- its vtable is provided by the derived leaves. Its
// primary subobject occupies +0x00..+0x0F (proven by the HelperAttack ctor writing the secondary
// I_CombatCollisionProcessOwner vtable at +0x10), i.e. it introduces no data members of its own.
//
// Verified derivation (from the C_CombatActionHelperAttack RTTI base array):
//   C_CombatActionHelperAttack : C_CombatActionHelper : C_CombatActorObject   (+ I_CombatCollisionProcessOwner @+0x10)
//   C_CombatActionHelperBlock  : C_CombatActionHelper : C_CombatActorObject   (+ secondary base @+0x10)
// The Hit helper does NOT use this base: C_CombatActionHelperHit : C_CombatActorObject directly.
//
// Leaf ctors (later wave -- addresses recorded here for that work):
//   C_CombatActionHelperAttack ctor sub_180915088  (vtables 0x183A61040 / 0x183A61028)
//   C_CombatActionHelperBlock  ctor sub_180C55794  (vtables 0x183A8A2D0 / 0x183A8A2B8)
//   C_CombatActionHelperHit    ctor sub_180D4A034  (vtable  0x183A99D48)

namespace wh::combatmodule {

// Abstract grouping base; adds virtual methods that the leaf helpers implement (not enumerated
// here -- they belong to the C_CombatActionHelperAttack/Block wave). No data members.
class C_CombatActionHelper : public C_CombatActorObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActionHelper;
};
static_assert(sizeof(C_CombatActionHelper) == 0x10, "C_CombatActionHelper must be 0x10 (== C_CombatActorObject)");

}  // namespace wh::combatmodule
