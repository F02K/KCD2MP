#pragma once
#include <cstdint>
#include "C_CombatActorUpdatedObject.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatTrigger -- base class of the combat timing-trigger family (KCD2 1.5.6, kd7u).  sizeof 0x18.
// -----------------------------------------------
// RTTI: .?AVC_CombatTrigger@combatmodule@wh@@ (TD 0x184B3CC60). Primary vtable 0x183A60DF8 (12 slots).
// Single-inheritance chain (all mdisp 0x00): C_CombatActorObject -> C_CombatActorUpdatedObject -> C_CombatTrigger.
// Abstract base -- always constructed as a subobject by a derived ctor (which calls the UpdatedObject ctor
// sub_180914754 inline); it owns NO data of its own (sizeof == UpdatedObject's 0x18). Each concrete trigger
// lays out its own fields from +0x18. dtor sub_1827CB83C just downgrades the vtable and forwards to the
// UpdatedObject dtor sub_180914714.
//
// vtable slots contributed here (over UpdatedObject's [0..9]):
//   [10] _vf10 -- default no-op (nullsub); overridden by SlotTrigger/Combo/... to sub_180914988 (cleanup helper)
//   [11] _vf11 -- PURE; returns an int trigger-kind/priority code (Combo->0, PB/Riposte->0xD observed in KCD1)
//
// C_CombatMultiTrigger<C_CombatActor, C_CombatSlotTrigger> and C_CombatActorMultiTrigger also derive from
// C_CombatTrigger (seen in dtor sub_180FC1238) but are SIBLINGS of the leaves in this cluster, not parents
// of them -- documented, deferred to a later wave.
//
// [vs KCD1] Same role/shape (KCD1 C_CombatTrigger: 12 slots [0..11], no own data). Only base offsets moved.

namespace wh::combatmodule {

class C_CombatTrigger : public C_CombatActorUpdatedObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatTrigger;
    virtual void _vf10() {}                 // [10] default no-op (KCD2 base = nullsub_1)
    virtual int  _vf11() const = 0;         // [11] PURE; int trigger-kind/priority (name UNVERIFIED)
    // No own data members (sizeof == C_CombatActorUpdatedObject).
};
static_assert(sizeof(C_CombatTrigger) == 0x18, "C_CombatTrigger adds virtuals but no data (== UpdatedObject)");

}  // namespace wh::combatmodule
