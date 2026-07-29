#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatCollisionProcessOwner -- collision-process owner interface
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable-only).
// -----------------------------------------------
// RTTI: .?AUI_CombatCollisionProcessOwner@combatmodule@wh@@  (TD 0x184AFABA0; abstract -- no
// standalone vtable/COL).  Declared with `struct` (mangled `U` prefix) even though polymorphic.
//
// KCD2-new interface: takes over the "collision owner" MI role that KCD1's attack helper owner
// carried.  Known implementors (secondary-vtable slot walk, both measure 2 slots):
//   C_CombatActionHelperAttack             @ +0x10  (vtable 0x183A61028: [0] 0x180D53B30, [1] 0x180C52730)
//   C_CombatActorActionSyncPerfectBlockHit @ +0x80  (vtable 0x183E409E0: [0] nullsub 0x1803B6E80,
//                                                    [1] 0x180C53564)
//
// Slot semantics (from the impls; names UNVERIFIED):
//   [0] acquire/refresh the owned collision process: HelperAttack's impl lazily fetches one from a
//       subsystem (+0x3A8 of the object at helper+0x08) via sub_180D53B74 and ref-count-swaps it
//       into helper+0x48 (AddRef vfunc[1]/Release vfunc[2] smart-ptr protocol), then registers it
//       (sub_18090C850).  SPBHit uses the do-nothing default.
//   [1] build a small collision-query descriptor into an out-param (returns the out pointer):
//       SPBHit's impl fills {int -1, 1.0f, 1.0f, int -1} then derives from the params (this-0x30
//       == action+0x50) via sub_180C5230C; HelperAttack forwards to vfunc[2] of the object at
//       helper+0x20.
//
// No virtual dtor slot (neither impl's [0] is a deleting dtor).

namespace wh::combatmodule {

struct I_CombatCollisionProcessOwner {
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatCollisionProcessOwner;
    virtual void  _vfAcquireCollisionProcess() = 0;        // [0] see header comment (name UNVERIFIED)
    virtual void* _vfGetCollisionQueryDesc(void* out) = 0; // [1] fills 0x10 out-desc (name UNVERIFIED)
};
static_assert(sizeof(I_CombatCollisionProcessOwner) == 0x08, "I_CombatCollisionProcessOwner is vtable-only");

}  // namespace wh::combatmodule
