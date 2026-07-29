#pragma once
#include <cstdint>
#include "C_CombatActorObject.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActionFactoryBase : C_CombatActorObject -- KCD2 WHGame.dll 1.5.6 (kd7u).
// sizeof 0x10 (adds no data over C_CombatActorObject).
// -----------------------------------------------
// Common base of the KCD2 per-type combat-action factories. RTTI
// .?AUC_CombatActionFactoryBase@combatmodule@wh@@ (TD 0x4AFA7D0). It has NO standalone COL/vtable
// (abstract intermediate: never instantiated on its own, and it introduces NO new virtual), so its
// vtable only materialises inside the concrete factory instantiations.
//
// [KCD1 -> KCD2 RESTRUCTURE]
//   KCD1 had ONE generic C_CombatActionFactory whose base carried a pure `CreateAction()` at
//   vtable[6] (overridden per factory kind). KCD2 split the factory into per-type leaves
//   (C_CombatActionAttackFactory / DodgeFactory / GuardFactory / PerfectBlockFactory) sharing this
//   C_CombatActionFactoryBase, AND REMOVED the virtual CreateAction: the concrete
//   C_CombatActionAttackFactory vtable (0x183BB7220) has exactly the 6 C_CombatActorObject slots --
//   slot [6] there is already the next class's COL (0x184107A28, sig==1, mdisp 8), proving the
//   factory adds no 7th virtual. Action creation in KCD2 is done by free dispatch functions that
//   take the factory as an enumeration CONTEXT (see C_CombatActionAttackFactory.h), not by a factory
//   virtual.
//
// Proof of size: derived ctor sub_181564AE0 allocates 16 (0x10) bytes, runs the C_CombatActorObject
// ctor sub_180914798(this, actor), then only writes the factory vtable at +0x00 -- no member past
// C_CombatActorObject's m_pOwner (+0x08).
//
// The 6 inherited C_CombatActorObject virtuals (concrete impls taken from the attack factory vtable):
//   [0] ~dtor                 sub_181645C08 (scalar-deleting, frees 0x10)
//   [1] Reset                 sub_1803B6E80 (nullsub)
//   [2] GetSubsystemId        sub_181A7D5B0 -> 37  (COMBAT_SUB_ACTION_FACTORY; KCD1 was 0x24=36)
//   [3] GetName               sub_181A7D440 -> "CombatActioFactory" (typo preserved from KCD1)
//   [4] GetDebugLabel         sub_1827603E4
//   [5] GetDebugInfo          sub_182757E04

namespace wh::combatmodule {

class C_CombatActionFactoryBase : public C_CombatActorObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActionFactoryBase;
    // Abstract intermediate: no new virtual, no new data. Concrete leaves override
    // GetSubsystemId()/GetName() (inherited from C_CombatActorObject).
    ~C_CombatActionFactoryBase() override = default;                 // [0]
};
static_assert(sizeof(C_CombatActionFactoryBase) == 0x10, "C_CombatActionFactoryBase must be 0x10");

}  // namespace wh::combatmodule
