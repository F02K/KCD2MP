#pragma once
#include <cstdint>
#include <cstddef>
#include "../animationmodule/C_CallbackAction.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyAnimAction -- the autocook brew-montage Mannequin action
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x270.
// -----------------------------------------------
// RTTI .?AVC_AlchemyAnimAction@playermodule@wh@@ (TD 0x184DD4828); vtable 0x183A47948.  Created
// by sub_1806C6CF0(this, ctx, priority=5, fragmentID, &tagState16B, 0): base init sub_180682DA8,
// then m_pContext = ctx and the "TargetPos" keyed param = the table entity's pos+rot
// (sub_1806C6D64 -> sub_1806825C4).  Dispatched to the player's Mannequin action controller by
// sub_1806C6C5C(playerCtx, &smartPtr).
//
// AUTOCOOK: the native Autocook executor (sub_180A95554) fills the ingredient slots then
// dispatches THIS action -- ONE canned montage: priority 5, fragment = **"AlchemyReturn"**
// (read in place from SMannequinHumanParams+140 at controller+0xA50; tag "alchemyBook" from
// +988 -- autocook_fragment_name.md), not per-verb playback.  Its completion callback (installed at
// m_onEnter via sub_180A96308, bound to a C_OpenBookAction member) advances the alchemy flow with
// result code 2 (sub_1806C2898 -> sub_1806C3A90(x,2) -> director AdvanceSlot), after which the
// brew grades through the NORMAL DSL/tolerance path (no special autocook score).

namespace wh::playermodule {

class C_AlchemyAnimAction : public wh::animationmodule::C_CallbackAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyAnimAction;

    void* m_pContext;   // +0x268  creator ctx (reading-flow object; holds the entity id at +0x28 for TargetPos)
};
static_assert(sizeof(C_AlchemyAnimAction) == 0x270, "C_AlchemyAnimAction must be 0x270");
static_assert(offsetof(C_AlchemyAnimAction, m_pContext) == 0x268, "ctx at 0x268");

}  // namespace wh::playermodule
