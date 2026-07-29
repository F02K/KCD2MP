#pragma once
#include <cstdint>
#include "C_CombatActorParamAction.h"
#include "I_CombatActorActionPrivate.h"
#include "S_CombatActorActionBlockTriggerParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionBlockTrigger -- the block-trigger action
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x78 (PROVEN: factory sub_181699558 allocates 120 bytes).
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionBlockTrigger@combatmodule@wh@@  (TD 0x184AFCA30; primary vtable
// 0x183BED088).  Combined alloc+ctor factory sub_181699558; base ctor sub_18169965C.
//
// NO C_CombatActorAnimatedAction / helper-owner layer -- the shortest block-family action.
// RTTI BaseClassArray (mdisp), most-derived first:
//   C_CombatActorActionBlockTrigger
//   C_CombatActorParamAction<S_CombatActorActionBlockTriggerParams,I_CombatActorActionPrivate>   @ +0x00
//     -> C_CombatActorAction -> C_ParamAction -> C_Action<I_CombatActorActionPrivate> @ +0x00
//        I_ActionImpl @ +0x10 ; C_CombatActorObject @ +0x58
// Sub-object offsets proved by the base ctor (params @ +0x50, C_CombatActorObject ctor sub_180914798
// at +0x58).  Default priority 2 (SetPriority sub_18090D1B0(this,2)).
//
// [vs KCD1] KCD1 sizeof 0xF8 -> 0x78.  Same own-field shape (state flags / data / flag).

namespace wh::combatmodule {

class C_CombatActorActionBlockTrigger
    : public C_CombatActorParamAction<S_CombatActorActionBlockTriggerParams, I_CombatActorActionPrivate>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionBlockTrigger;
    // ---- own members (leaf data begins at +0x70; the +0x68/+0x6C pair belongs to the
    //      C_CombatActorAction<> wrapper -- see C_CombatActorAction.h) ----
    bool     m_flag;         // +0x70  init 0
    uint8_t  _pad71[7];      // +0x71
};
static_assert(sizeof(C_CombatActorActionBlockTrigger) == 0x78, "C_CombatActorActionBlockTrigger must be 0x78");

}  // namespace wh::combatmodule
