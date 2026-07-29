#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"
#include "I_CombatActorActionPrivate.h"
#include "I_CombatActionHelperBlockOwner.h"
#include "I_CombatActionHelperAttackOwner.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionPerfectBlockBase<TParams, N> -- shared base of the
// perfect-block actions (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI ?$C_CombatActorActionPerfectBlockBase@... (from the PerfectBlock/SyncPerfectBlock leaf base
// arrays; survives from KCD1 with the same role). Carries BOTH helper-owner MI bases (block +
// attack) because a perfect block can chain into a riposte attack.
//   C_CombatActorActionPerfectBlock     : PerfectBlockBase<S_CombatActorActionPerfectBlockParams, 2>
//   C_CombatActorActionSyncPerfectBlock : PerfectBlockBase<S_CombatActorActionSyncPerfectBlockParams, 1>
// Own-data ctor sub_18164F188. sizeof(PerfectBlockBase<PerfectBlockParams 0x30, 2>) == 0xE8,
// <SyncPerfectBlockParams 0x30, 1> == 0xE0 (leaf allocs 0xF0 / 0xF8 minus leaf tails).
// Sets default priority 4 (SetPriority sub_18090D1B0(this, 4)).
//
// Absolute offsets below are for the N=2 / params 0x30 instantiation (PerfectBlock):
// AnimatedAction ends +0xA8, BlockOwner @+0xA8, AttackOwner @+0xB0, own data +0xB8..+0xE8.

namespace wh::combatmodule {

class C_CombatActionHelperBlock;    // owned block helper  (alloc 0x38; helper wave)
class C_CombatActionHelperAttack;   // owned attack helper (alloc 0x50; ctor sub_180915088)

template<typename TParams, int N>
class C_CombatActorActionPerfectBlockBase
    : public C_CombatActorAnimatedAction<TParams, N, I_CombatActorActionPrivate>,
      public I_CombatActionHelperBlockOwner,
      public I_CombatActionHelperAttackOwner
{
public:
    // ---- own data (ctor sub_18164F188; offsets rel. to the N=2/0x30 instantiation) ----
    uint32_t m_flagB8;                            // +0xB8  (ctor 0; semantics UNVERIFIED)
    uint32_t m_flagBC;                            // +0xBC  (ctor 0; semantics UNVERIFIED)
    uint32_t m_flagC0;                            // +0xC0  (ctor 0; semantics UNVERIFIED)
    uint32_t _padC4;                              // +0xC4
    C_CombatActionHelperBlock*  m_pBlockHelper;   // +0xC8  owned (alloc 0x38)
    C_CombatActionHelperAttack* m_pAttackHelper;  // +0xD0  owned (alloc 0x50)
    C_CombatActionHelperBlock*  m_pBlockHelperCopy; // +0xD8  ctor'd == m_pBlockHelper (non-owning copy)
    uint32_t m_dwordE0;                           // +0xE0  (ctor 0; semantics UNVERIFIED)
    uint32_t _padE4;                              // +0xE4
};

}  // namespace wh::combatmodule
