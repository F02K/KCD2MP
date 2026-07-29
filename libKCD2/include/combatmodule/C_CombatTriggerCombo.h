#pragma once
#include "C_CombatSlotTrigger.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatTriggerCombo -- combo follow-up window trigger (KCD2 1.5.6, kd7u).  sizeof 0x48.
// -----------------------------------------------
// RTTI: .?AVC_CombatTriggerCombo@combatmodule@wh@@ (TD 0x184B3C068). Primary vtable 0x183AED038 (18 slots).
// ctor sub_180FC13B4 (alloc 0x48 == PROVEN sizeof; calls SlotTrigger ctor with updateFlags=2, then registers
// a combo callback into the actor's combo manager); dtor sub_180FC1370. Direct base C_CombatSlotTrigger.
// Adds NO new vtable slots and NO own data (same 18 slots / 0x48 as SlotTrigger) -- it only OVERRIDES:
// dtor[0], GetName[3] (sub_181A7F7B0), [7], [11] (sub_18066CD10 -> 0), OnActivate[14] (sub_18180C540),
// OnDeactivate[15] (sub_18180C55C).
//
// [vs KCD1] Same shape (18 slots, frees 0x48). OnActivate/OnDeactivate flip the combo-active state on the
// owning actor's combat state and fire its combo-window signal.

namespace wh::combatmodule {

class C_CombatTriggerCombo : public C_CombatSlotTrigger {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatTriggerCombo;
    // No new virtuals, no own data (overrides only). sizeof inherited from C_CombatSlotTrigger.
};
static_assert(sizeof(C_CombatTriggerCombo) == 0x48, "ctor sub_180FC13B4 allocates 0x48");

}  // namespace wh::combatmodule
