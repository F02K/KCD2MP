#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"
#include "I_CombatActorActionPrivate.h"
#include "I_CombatCollisionProcessOwner.h" // DEPENDENCY: attack/collision-side interface base
#include "S_CombatActorActionSyncPerfectBlockHitParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionSyncPerfectBlockHit -- the sync-perfect-block "hit" action
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xE0 (PROVEN: factory sub_1810F3148 allocates 224 bytes).
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionSyncPerfectBlockHit@combatmodule@wh@@  (TD 0x184AFBC50; primary vtable
// 0x183E40740).  ctor sub_1810F3204; factory sub_1810F3148; deleting dtor sub_1810F3438.
//
// Uses C_CombatActorAnimatedAction directly (NO PerfectBlockBase layer); its extra MI base is
// I_CombatCollisionProcessOwner (this is the "additional MI subobject vtable" seen in KCD1 at +0xF8,
// now at +0x80).  RTTI BaseClassArray (mdisp), most-derived first:
//   C_CombatActorActionSyncPerfectBlockHit
//   C_CombatActorAnimatedAction<S_CombatActorActionSyncPerfectBlockHitParams,1,I_CombatActorActionPrivate> @ +0x00
//     -> ... -> C_Action @ +0x00 ; I_ActionImpl @ +0x10 ; C_CombatActorObject @ +0x60
//   I_CombatCollisionProcessOwner   @ +0x80   (struct; attack/collision side)
// AnimatedAction<...,1> ends at +0x80, so the leaf tail begins at +0x88.  Default priority 9.
// The action factory also fills the params: *(+0x50)=arg (params+0x00), *(+0x58)=byte (params+0x08).
//
// [vs KCD1] KCD1 sizeof 0x118 -> 0xE0.  The Hit action EMBEDS a C_CombatActionHelperHit by value at
// +0xA8 (ctor sub_180D4A034, sizeof 0x30, vtable 0x183A99D48) and separately owns a C_CombatActionEarlyExitHelper (KCD1: C_CombatActionSyncHelper).

namespace wh::combatmodule {

class C_CombatActionEarlyExitHelper;   // owned early-exit/sync helper (alloc 0x18, ctor sub_180914FA4; RTTI-named)

class C_CombatActorActionSyncPerfectBlockHit
    : public C_CombatActorAnimatedAction<S_CombatActorActionSyncPerfectBlockHitParams, 1, I_CombatActorActionPrivate>,
      public I_CombatCollisionProcessOwner
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionSyncPerfectBlockHit;
    // ---- own members (leaf tail begins at +0x88, after the I_CombatCollisionProcessOwner base) ----
    uint32_t                  m_field88;     // +0x88  uint32 query-id: while zero, OnTick 0x180D4BB64 caches HIDWORD of a subsystem vf[0x118] result (same pattern as SyncHit m_unknown_80)
    uint16_t                  m_field8C;     // +0x8C  two byte-flags: low byte (+0x8C) tested in OnStop 0x180E7B032, high byte (+0x8D) gates OnStart step calc at 0x18155EB39
    uint8_t                   _pad8E[2];     // +0x8E
    C_CombatActionEarlyExitHelper* m_pSyncHelper; // +0x90  owned; alloc 0x18 in ctor; deleting-dtor'd
    float                     m_field98;     // +0x98  float; OnStart 0x18155EBA6 stores (v5 + anim-offset), later read as float and epsilon-tested (>1.19e-7) at 0x18155ED85
    uint8_t                   _pad9C[4];     // +0x9C
    C_CombatActorActionSyncPerfectBlock* m_pSyncOwner; // +0xA0  raw weak back-ref to the owning block action (set by 0x1810F30E5 partner->+0xA0=block; checked/nulled in OnStop 0x180E7B055; cleared by the block dtor at 0x18236822C)
    // embedded C_CombatActionHelperHit by value (ctor sub_180D4A034; sizeof 0x30; vtable 0x183A99D48).
    // Held as a raw buffer here so this header's size claim is self-contained.
    alignas(8) uint8_t        m_hitHelper[0x30];   // +0xA8  [+0xA8..+0xD8)
    void*                     m_pTaskHandle; // +0xD8  VERIFIED: holds Concurrency::details::_AutoDeleter<_TaskProcHandle> (8B); task scheduled in OnStart 0x18155EC8A, ~_AutoDeleter in dtor 0x1810F3497
};
static_assert(sizeof(C_CombatActorActionSyncPerfectBlockHit) == 0xE0, "C_CombatActorActionSyncPerfectBlockHit must be 0xE0");

}  // namespace wh::combatmodule
