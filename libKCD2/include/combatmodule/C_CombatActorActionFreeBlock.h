#pragma once
#include <cstdint>
#include "C_CombatActorParamAction.h"
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionFreeBlockParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionFreeBlock -- the free-block action
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x80 (PROVEN: factory sub_182749C8C allocates 128 bytes).
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionFreeBlock@combatmodule@wh@@  (TD 0x184AFA190; primary vtable
// 0x183E40F80).  Combined alloc+ctor factory sub_182749C8C; base ctor sub_182753C68.
//
// NO C_CombatActorAnimatedAction / helper-owner layer.  RTTI BaseClassArray (mdisp), most-derived first:
//   C_CombatActorActionFreeBlock
//   C_CombatActorParamAction<S_CombatActorActionFreeBlockParams,I_CombatActorActionPrivate>       @ +0x00
//     -> C_CombatActorAction -> C_ParamAction -> C_Action<I_CombatActorActionPrivate> @ +0x00
//        I_ActionImpl @ +0x10 ; C_CombatActorObject @ +0x58
// Default priority 2.
//
// [vs KCD1] KCD1 sizeof 0x100 -> 0x80.  Same own-field shape (state flags/data, a ref-counted object
// pointer, and a time sentinel).  Note the timer is stored as a 64-bit -100000 sentinel (int64), same
// as C_CombatActorActionBlock (KCD1 used a float -100000.0f).

namespace wh::combatmodule {

class C_CombatActorActionGuardMovement;   // RTTI .?AVC_CombatActorActionGuardMovement@combatmodule@wh@@

class C_CombatActorActionFreeBlock
    : public C_CombatActorParamAction<S_CombatActorActionFreeBlockParams, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionFreeBlock;
    // ---- own members (leaf data begins at +0x70; the +0x68/+0x6C pair belongs to the
    //      C_CombatActorAction<> wrapper -- see C_CombatActorAction.h) ----
    _smart_ptr<C_CombatActorActionGuardMovement> m_pGuardMovement; // +0x70  owned; OnStart sub_182755718 @0x182755775 move-assigns (sub_180521954) the guard-movement action built by sub_18090CA88 (alloc 0x78); reset via sub_180C5385C (null+slot2 Release) @0x1827557CE
    int64_t  m_blockTime;       // +0x78  init int64 -100000 (time/frame sentinel; UNVERIFIED name)
};
static_assert(sizeof(C_CombatActorActionFreeBlock) == 0x80, "C_CombatActorActionFreeBlock must be 0x80");

}  // namespace wh::combatmodule
