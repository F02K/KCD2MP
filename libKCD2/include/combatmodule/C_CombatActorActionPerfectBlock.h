#pragma once
#include <cstdint>
#include "C_CombatActorActionPerfectBlockBase.h"
#include "S_CombatActorActionPerfectBlockParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionPerfectBlock -- the perfect-block action
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xF0 (PROVEN: factory sub_18164F0E4 allocates 240 bytes).
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionPerfectBlock@combatmodule@wh@@  (TD 0x184AFC2B0; primary vtable
// 0x183BE12C0).  factory sub_18164F0E4; PerfectBlockBase ctor sub_18164F188; params ctor sub_18164F43C.
//
// The perfect-block-specific state (two owned helpers + the two helper-OWNER interfaces) lives in the
// shared intermediate C_CombatActorActionPerfectBlockBase<TParams,N> -- confirmed present in KCD2 (it
// was NOT removed).  The leaf itself adds only a small trailing tail.
//
// RTTI BaseClassArray (mdisp), most-derived first:
//   C_CombatActorActionPerfectBlock
//   C_CombatActorActionPerfectBlockBase<S_CombatActorActionPerfectBlockParams,2>                @ +0x00
//     -> C_CombatActorAnimatedAction<...,2,I_CombatActorActionPrivate> -> ... -> C_Action @ +0x00
//        I_ActionImpl @ +0x10 ; C_CombatActorObject @ +0x80
//   I_CombatActionHelperBlockOwner   @ +0xA8   (contributed by PerfectBlockBase)
//   I_CombatActionHelperAttackOwner  @ +0xB0   (contributed by PerfectBlockBase)
// PerfectBlockBase own data (from sub_18164F188): flags @ +0xB8/+0xBC/+0xC0, m_pBlockHelper @ +0xC8
// (alloc 0x38), m_pAttackHelper @ +0xD0 (alloc 0x50), m_pBlockHelperCopy @ +0xD8, dword @ +0xE0.
// sizeof(PerfectBlockBase<...,2>) == 0xE8, so the leaf tail begins at +0xE8.  Default priority 4.
//
// [vs KCD1] KCD1 sizeof 0x150 -> 0xF0.  KCD1 leaf was empty; KCD2 leaf carries a small tail. KCD1 N=1,
// KCD2 N=2 (the animated-action callback count grew).

namespace wh::combatmodule {

class C_CombatActorActionPerfectBlock
    : public C_CombatActorActionPerfectBlockBase<S_CombatActorActionPerfectBlockParams, 2>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionPerfectBlock;
    // ---- own members (leaf tail begins at +0xE8, after C_CombatActorActionPerfectBlockBase) ----
    uint16_t m_fieldE8;      // +0xE8  init 0
    bool     m_flagEA;       // +0xEA  init 0
    uint8_t  _padEB[5];      // +0xEB
};
static_assert(sizeof(C_CombatActorActionPerfectBlock) == 0xF0, "C_CombatActorActionPerfectBlock must be 0xF0");

}  // namespace wh::combatmodule
