#pragma once
#include <cstdint>
#include "C_CombatActorAnimatedAction.h"
#include "I_CombatActorActionBlock.h"
#include "I_CombatActionHelperBlockOwner.h"
#include "S_CombatActorActionBlockParams.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorActionBlock -- the block combat action (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0xB0 (PROVEN: factory sub_180C559F8 allocates 176 bytes).
// -----------------------------------------------
// RTTI: .?AVC_CombatActorActionBlock@combatmodule@wh@@  (TD 0x184AFD748; primary vtable 0x183A89688).
// ctor sub_180C55608; factory sub_180C559F8; deleting dtor sub_180C55720.
//
// UNIQUE among the actions: it is the ONLY one parameterised on I_CombatActorActionBlock (the +2-slot
// combat interface), so its C_Action base is C_Action<I_CombatActorActionBlock> (67-slot vtable
// 0x183A89F48) -- everyone else uses C_Action<I_CombatActorActionPrivate> (65 slots).
//
// RTTI BaseClassArray (mdisp), most-derived first:
//   C_CombatActorActionBlock
//   C_CombatActorAnimatedAction<S_CombatActorActionBlockParams,2,I_CombatActorActionBlock>  @ +0x00
//     -> C_CombatActorParamAction -> C_CombatActorAction -> C_ParamAction -> C_Action<...Block> @ +0x00
//        I_ActionImpl @ +0x10 ; C_CombatActorObject @ +0x60
//   I_CombatActionHelperBlockOwner                                                          @ +0x88
// Sub-object offsets proved by the ctor's vtable stores (+0x00/+0x10/+0x60/+0x88).
//
// [vs KCD1] KCD1 sizeof 0x128; the object shrank to 0xB0 (C_Action 0xD0->0x50, single signal, etc.).
// The block helper alloc grew 0x28 -> 0x38.

namespace wh::combatmodule {

class C_CombatActionHelperBlock;   // owned helper leaf (ctor sub_180C55794, alloc 0x38); RE'd separately

class C_CombatActorActionBlock
    : public C_CombatActorAnimatedAction<S_CombatActorActionBlockParams, 2, I_CombatActorActionBlock>,
      public I_CombatActionHelperBlockOwner
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorActionBlock;
    // ---- own members (leaf data begins at +0x90, after the I_CombatActionHelperBlockOwner base) ----
    int64_t                    m_blockTimeStart;   // +0x90  init int64 -100000 (frame/time sentinel; UNVERIFIED name)
    int64_t                    m_blockTimeEnd;     // +0x98  init int64 -100000 (frame/time sentinel; UNVERIFIED name)
    C_CombatActionHelperBlock* m_pBlockHelper;     // +0xA0  owned; alloc 0x38 in ctor; deleting-dtor'd
    bool                       m_blockFlag;        // +0xA8  init 0
    uint8_t                    _padA9[7];          // +0xA9
};
static_assert(sizeof(C_CombatActorActionBlock) == 0xB0, "C_CombatActorActionBlock must be 0xB0");

}  // namespace wh::combatmodule
