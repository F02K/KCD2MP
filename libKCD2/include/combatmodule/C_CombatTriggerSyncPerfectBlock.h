#pragma once
#include "C_CombatTriggerPerfectBlock.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatTriggerSyncPerfectBlock -- synchronized (2-actor) perfect block (KCD2 1.5.6, kd7u).  sizeof 0x48.
// -----------------------------------------------
// RTTI: .?AVC_CombatTriggerSyncPerfectBlock@combatmodule@wh@@ (TD 0x184B3BD70). Primary vtable 0x183B815E0 (22 slots).
// ctor sub_181411204 (alloc 0x48 == PROVEN sizeof; forwards to the PerfectBlock sub-ctor sub_180915270);
// dtor sub_1814111BC. Direct base C_CombatTriggerPerfectBlock (RTTI chain: ...PerfectBlock -> SlotTrigger ->
// C_CombatTrigger -> UpdatedObject -> C_CombatActorObject, all mdisp 0x00). Adds NO new vtable slots (same 22
// as PerfectBlock) and NO own data -- it only OVERRIDES the PB behaviour for the two-actor synced case
// (e.g. [11] sub_181A724F0, [14] sub_181809C04, [15] sub_1817C6EC8, [20] sub_181A78EA0, [21] sub_1819B0660).
//
// [vs KCD1] KCD1 modelled this deriving from C_CombatSlotTrigger; the KCD2 RTTI proves the true parent is
// C_CombatTriggerPerfectBlock. NOTE: KCD1's C_CombatTriggerSyncDodge and C_CombatTriggerSyncRiposte are GONE
// in KCD2 (no type descriptors) -- removed, no headers produced.

namespace wh::combatmodule {

class C_CombatTriggerSyncPerfectBlock : public C_CombatTriggerPerfectBlock {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatTriggerSyncPerfectBlock;
    // No new virtuals, no own data (overrides only). sizeof inherited from C_CombatTriggerPerfectBlock.
};
static_assert(sizeof(C_CombatTriggerSyncPerfectBlock) == 0x48, "ctor sub_181411204 allocates 0x48");

}  // namespace wh::combatmodule
