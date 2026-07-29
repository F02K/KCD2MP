#pragma once
#include <cstdint>
#include "C_CombatActorParamAction.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorAnimatedAction<TParams, N, TPrivate> -- animated combat action
// base (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Third of the three intermediate templates that replaced KCD1's C_CombatActorActionT:
//   C_CombatActorAnimatedAction<TParams, int N, TPrivate> : C_CombatActorParamAction<TParams, TPrivate>
// N (mangled $00 = 1, $01 = 2) is the number of 8-byte animation-scope slots appended after the
// C_CombatActorAction wrapper fields -- proven by the leaf size matrix: every N=1 leaf ends
// 0x50 + sizeof(TParams) + 0x20; Hit/PerfectBlock (N=2) add exactly +0x08 (leaf-data start shifts
// by 8, e.g. Hit vs SyncTransition). The slot is constructed by e.g. sub_18090D378(this+0x80) in
// the Attack base-chain ctor; zero-init, likely a smart-ptr/handle to the anim scope
// [element semantics UNVERIFIED].
//
// Appends ONE primary vtable slot after the combat wrapper's [65]/[66]:
//   [67] default = nullsub 0x1803B6E80 in every measured leaf (Attack/GuardMovement/Hit/
//        PerfectBlock) -- an optional animation hook. Leaves with the Animated layer have 68-slot
//        primary vtables; plain-ParamAction leaves (Guard) have 67 (proven on kd7u).
//
// Almost every action leaf derives from this (N=1); Hit and PerfectBlock use N=2. Exceptions
// (plain C_CombatActorParamAction): Guard, BlockTrigger, FreeBlock.

namespace wh::combatmodule {

template<typename TParams, int N, typename TPrivate>
class C_CombatActorAnimatedAction : public C_CombatActorParamAction<TParams, TPrivate> {
public:
    virtual void _vfAnimHook() = 0;    // [67] default nullsub 0x1803B6E80 (name UNVERIFIED)

    uint64_t m_animScopes[N];          // 8-byte anim-scope slot(s); ctor'd by e.g. sub_18090D378 (Attack chain)
};

}  // namespace wh::combatmodule
