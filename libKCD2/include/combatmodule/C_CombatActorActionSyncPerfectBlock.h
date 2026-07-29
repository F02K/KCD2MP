#pragma once
#include <cstdint>
#include "C_CombatActorActionPerfectBlockBase.h"
#include "S_CombatActorActionSyncPerfectBlockParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionSyncPerfectBlock -- the synchronised perfect-block action
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xF8 (PROVEN: factory sub_1814840F4 allocates 248 bytes).
// The largest block-family action.
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionSyncPerfectBlock@combatmodule@wh@@  (TD 0x184AFB560; primary vtable
// 0x183B933B0).  ctor sub_181484180; PerfectBlockBase ctor sub_1814842F8; deleting dtor sub_18148425C.
//
// Shares the SAME intermediate C_CombatActorActionPerfectBlockBase<TParams,N> as PerfectBlock, but with
// N=1 (so every helper-owner/base offset is 0x08 lower).  RTTI BaseClassArray (mdisp), most-derived first:
//   C_CombatActorActionSyncPerfectBlock
//   C_CombatActorActionPerfectBlockBase<S_CombatActorActionSyncPerfectBlockParams,1>            @ +0x00
//     -> C_CombatActorAnimatedAction<...,1,I_CombatActorActionPrivate> -> ... -> C_Action @ +0x00
//        I_ActionImpl @ +0x10 ; C_CombatActorObject @ +0x80
//   I_CombatActionHelperBlockOwner   @ +0xA0
//   I_CombatActionHelperAttackOwner  @ +0xA8
// sizeof(PerfectBlockBase<...,1>) == 0xE0, so the leaf tail begins at +0xE0.  Default priority 4.
//
// [vs KCD1] KCD1 sizeof 0x160 -> 0xF8.  Leaf still owns an early-exit/sync helper (C_CombatActionEarlyExitHelper) plus a ref-counted
// sync partner released in the deleting dtor.

namespace wh::combatmodule {

class C_CombatActionEarlyExitHelper;   // owned early-exit/sync helper (alloc 0x18, ctor sub_180914FA4; RTTI-named)
class C_CombatActorActionSyncPerfectBlockHit;   // paired hit action (see C_CombatActorActionSyncPerfectBlockHit.h)

class C_CombatActorActionSyncPerfectBlock
    : public C_CombatActorActionPerfectBlockBase<S_CombatActorActionSyncPerfectBlockParams, 1>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionSyncPerfectBlock;
    // ---- own members (leaf tail begins at +0xE0, after C_CombatActorActionPerfectBlockBase<...,1>) ----
    uint16_t                 m_syncFlags;    // +0xE0  init 0
    bool                     m_syncFlag;     // +0xE2  init 0
    uint8_t                  _padE3[5];      // +0xE3
    C_CombatActionEarlyExitHelper* m_pSyncHelper; // +0xE8  owned; alloc 0x18 in ctor; deleting-dtor'd
    _smart_ptr<C_CombatActorActionSyncPerfectBlockHit> m_pSyncPartner; // +0xF0  owns the paired hit action; created by factory sub_1810F3148 and moved-in (sub_180521954) by sub_1810F3030, which also sets partner->+0xA0 = this; Released (slot2) in dtor 0x1814842F1
};
static_assert(sizeof(C_CombatActorActionSyncPerfectBlock) == 0xF8, "C_CombatActorActionSyncPerfectBlock must be 0xF8");

}  // namespace wh::combatmodule
