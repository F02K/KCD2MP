#pragma once
#include <cstdint>
#include "../framework/C_ActionDirector.h"
#include "../CryEngine/CryCommon/smartptr.h"
#include "E_AlchemyDirectorSlot.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyActionDirector -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x90.
// -----------------------------------------------
// RTTI .?AVC_AlchemyActionDirector@playermodule@wh@@ (TD 0x184DD4BE8); vtable 0x183BFA5B0.
// The alchemy session's per-slot action driver, EMBEDDED at C_Alchemy+0x78 and spanning exactly
// +0x78..+0x108 (== sizeof(framework::C_ActionDirector) -- adds NO own data fields).  Constructed
// by the C_Alchemy ctor as sub_1809CDAD0(this+0x78, mode=3) (E_ActionDirectorMode minigame mode).
//
// Slot model: each action's director SLOT = its m_actionSequenceId (+0x3C; alchemy base ctor
// writes 1, C_SwitchPotHalterAction writes 4 for the parallel pot-halter channel).  A new action
// REPLACES/merges whatever occupies its slot (priority resolve sub_1809CC5F0).  The tick drives
// each occupied slot through the action's I_ActionImpl (AsActionImpl [22]): keeps it while
// IsStarted, steps it, clears the slot when the step reports finished (anim-event-driven).
//
// m_actions (base +0x38, = C_Alchemy+0xB0) is the live per-slot vector consulted by
// IsSlotOccupied; the tick entry sub_180737A7C is called from the session input-drain
// sub_180737920 each frame.

namespace wh::playermodule {

class I_AlchemyAction;

class C_AlchemyActionDirector : public wh::framework::C_ActionDirector {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyActionDirector;

    // True if the slot holds a live action (queue = m_actions; bounds-checked).
    bool IsSlotOccupied(E_AlchemyDirectorSlot::Type slot) const;      // 0x1808D2450
    // Insert/replace the action into its GetActionSequenceId() slot (priority resolve + listener notify).
    void PushAction(_smart_ptr<I_AlchemyAction>& spAction);           // 0x1809CC3D4
    // Per-frame driver: step every occupied slot via its I_ActionImpl; clear finished slots.
    void Tick();                                                      // 0x180737A7C
    // Advance/complete the slot's action with a result code (2 = success-advance; used by anim
    // completion callbacks, e.g. autocook's montage cb sub_1806C3A90(x,2)).
    void AdvanceSlot(E_AlchemyDirectorSlot::Type slot, int32_t resultCode);  // 0x1809CD780
};
static_assert(sizeof(C_AlchemyActionDirector) == 0x90, "C_AlchemyActionDirector adds no fields");

}  // namespace wh::playermodule
