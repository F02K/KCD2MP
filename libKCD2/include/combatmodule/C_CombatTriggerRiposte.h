#pragma once
#include "C_CombatSlotTrigger.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatTriggerRiposte -- master-strike (riposte) window (KCD2 1.5.6, kd7u).  sizeof 0x48.
// -----------------------------------------------
// RTTI: .?AVC_CombatTriggerRiposte@combatmodule@wh@@ (TD 0x184B3C640). Primary vtable 0x183AECFA0 (18 slots).
// ctor sub_180FC12EC (alloc 0x48 == PROVEN sizeof; calls SlotTrigger ctor with updateFlags=2); dtor
// sub_180FC0E6C. Direct base C_CombatSlotTrigger. Adds NO new vtable slots and NO own data (18 slots / 0x48).
// Overrides: dtor[0], GetName[3] (sub_181A7F7E0), [7], [11] (sub_181A74D10 -> 0xD in KCD1), [12]
// (sub_1819DE250), OnActivate[14] (sub_1817DF914), OnDeactivate[15] (sub_1819A9F4C), [16] (sub_1827CD158).
//
// [vs KCD1] Same shape (18 slots, frees 0x48). OnActivate/OnDeactivate flip the actor's riposte-active state
// and fire its riposte-window signal.

namespace wh::combatmodule {

class C_CombatTriggerRiposte : public C_CombatSlotTrigger {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatTriggerRiposte;
    // No new virtuals, no own data (overrides only). sizeof inherited from C_CombatSlotTrigger.
};
static_assert(sizeof(C_CombatTriggerRiposte) == 0x48, "ctor sub_180FC12EC allocates 0x48");

}  // namespace wh::combatmodule
