#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatActionHelperBlockOwner -- "owner" interface implemented by any action that
// drives a C_CombatActionHelperBlock (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable pointer only).
// -----------------------------------------------
// RTTI: .?AVI_CombatActionHelperBlockOwner@combatmodule@wh@@  (class; TD 0x184AFC940, COL 0x184160E48,
// standalone vtable 0x183A8A168).
//
// Appears as an ADDITIONAL (multiple-inheritance) base of the block/perfect-block actions:
//   C_CombatActorActionBlock                        base @ +0x88
//   C_CombatActorActionPerfectBlockBase<...,2>      base @ +0xA8  (inside PerfectBlock)
//   C_CombatActorActionPerfectBlockBase<...,1>      base @ +0xA0  (inside SyncPerfectBlock)
// (proved by each leaf's RTTI BaseClassArray mdisp).  The helper's ctor (C_CombatActionHelperBlock,
// sub_180C55794) is handed `this-as-owner` (the +0x88/+0xA8/+0xA0 subobject) so it can call back.
// Adds NO data members.
//
// Slot map (base vtable 0x183A8A168):
//   [0] 0x182754E28  ~dtor (scalar deleting)
//   [1] 0x181D93E5D  _purecall  (leaf-implemented)
//   [2] 0x181D93E5D  _purecall  (leaf-implemented)
//   [3] 0x1803B6E80  nullsub    (base default)
//   [4] 0x181D93E5D  _purecall  (leaf-implemented)
// Names are not recoverable from the abstract base (impls live in the action leaves) -- declared _vfN.

namespace wh::combatmodule {

class I_CombatActionHelperBlockOwner {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActionHelperBlockOwner;
    virtual ~I_CombatActionHelperBlockOwner();   // [0] 0x182754E28
    virtual void _vf1() = 0;                      // [1] _purecall  (leaf-implemented)
    virtual void _vf2() = 0;                      // [2] _purecall  (leaf-implemented)
    virtual void _vf3();                          // [3] 0x1803B6E80 nullsub (base default)
    virtual void _vf4() = 0;                      // [4] _purecall  (leaf-implemented)
    // No data members.
};
static_assert(sizeof(I_CombatActionHelperBlockOwner) == 0x08,
              "I_CombatActionHelperBlockOwner is vtable-only (empty polymorphic base)");

}  // namespace wh::combatmodule
