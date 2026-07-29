#pragma once

// -----------------------------------------------
// wh::combatmodule::I_CombatActionHelperAttackOwner -- callback interface a C_CombatActionHelperAttack
// uses to talk back to the action that owns it (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI: .?AVI_CombatActionHelperAttackOwner@combatmodule@wh@@  (TD present; abstract -- no standalone
// vtable). Pure interface: no data members (just the vtable pointer).
//
// It is a SECONDARY MI base of the three attack-owning leaf actions (subobject offset per leaf):
//   C_CombatActorActionAttack        @ +0x88   impl vtable 0x183A61440
//   C_CombatActorActionSyncAttack    @ +0x88
//   C_CombatActorActionStaticAttack  @ +0x80
// The owning action passes `this` I_CombatActionHelperAttackOwner subobject into the helper ctor
// (sub_180915088, C_CombatActionHelperAttack, alloc 0x50) as the back-reference owner.
//
// The vtable has 8 slots. Handlers below are the C_CombatActorActionAttack impls (adjustor thunks into
// the leaf at -0x88). Method names are not statically recoverable from the abstract base -- declared
// `_vfN` with the leaf handler address per slot. Slot [0] is the (adjustor) destructor thunk.

namespace wh::combatmodule {

class I_CombatActionHelperAttackOwner {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActionHelperAttackOwner;
    virtual ~I_CombatActionHelperAttackOwner() = default;  // [0] 0x182138F50 (adjustor -0x88 -> leaf dtor)
    virtual void _vf1() = 0;   // [1] 0x181A75F50 (nullsub / trivial return)
    virtual void _vf2() = 0;   // [2] 0x180C52CC8
    virtual void _vf3() = 0;   // [3] 0x182139118 (adjustor thunk)
    virtual void _vf4() = 0;   // [4] 0x180D4C1FC
    virtual void _vf5() = 0;   // [5] 0x18164A5E0
    virtual void _vf6() = 0;   // [6] 0x180973380
    virtual void _vf7() = 0;   // [7] 0x181A7D940 (nullsub / trivial return)
    // No data members.
};
static_assert(sizeof(I_CombatActionHelperAttackOwner) == 0x08,
              "I_CombatActionHelperAttackOwner = vtable pointer only");

}  // namespace wh::combatmodule
