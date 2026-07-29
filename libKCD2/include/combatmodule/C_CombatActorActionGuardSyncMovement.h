#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionGuardSyncMovementParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionGuardSyncMovement -- synced guard-movement leaf (KCD2 1.5.6, kd7u).  sizeof 0x80.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionGuardSyncMovement@combatmodule@wh@@ (TD 0x184AFBF80).
// ctor sub_18090C368 (invoked by allocating factory sub_18090C2DC, alloc 0x80 == PROVEN sizeof);
// dtor sub_18090C448. Vtables: +0x00 0x183A5F3C0 | +0x10 0x183A5EC70 (I_ActionImpl) | +0x58 0x183A5EBC0.
//
// TRUE Warhorse hierarchy (RTTI BaseClassArray of TD 0x184AFBF80): identical wrapper chain to
// GuardMovement but parameterised with S_CombatActorActionGuardSyncMovementParams:
//   C_CombatActorActionGuardSyncMovement
//     : C_CombatActorAnimatedAction<S_...GuardSyncMovementParams, 1, I_CombatActorActionPrivate>
//         : C_CombatActorParamAction<...> : C_CombatActorAction<...>
//             : C_ParamAction<..., C_Action<...>> (mdisp 0x00) : C_CombatActorObject (mdisp 0x58)
// Wrapper templates live in C_CombatActorAnimatedAction.h. The one difference vs. GuardMovement:
// the ctor heap-allocates a C_CombatActionEarlyExitHelper (0x18 via sub_180914FA4) and stores the pointer
// at +0x78 -- this is the leaf's single own member (KCD1 called the equivalent m_pSyncHelper).
//
// [vs KCD1] KCD1 was 0x100 (m_pSyncHelper -> C_CombatActionSyncHelper). KCD2 0x80; the sync helper type
// is now C_CombatActionEarlyExitHelper (same class the SlotTrigger family also embeds).

namespace wh::combatmodule {

class C_CombatActionEarlyExitHelper;   // foreign (0x18; ctor sub_180914FA4) -- dependency, RE'd elsewhere

class C_CombatActorActionGuardSyncMovement
    : public C_CombatActorAnimatedAction<S_CombatActorActionGuardSyncMovementParams, 1, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionGuardSyncMovement;
    // ---- C_CombatActorActionGuardSyncMovement own data (+0x78..+0x80) ----
    C_CombatActionEarlyExitHelper* m_pEarlyExitHelper; // +0x78  heap-allocated 0x18 helper (sub_180914FA4)
};
static_assert(sizeof(C_CombatActorActionGuardSyncMovement) == 0x80, "factory sub_18090C2DC allocates 0x80");

}  // namespace wh::combatmodule
