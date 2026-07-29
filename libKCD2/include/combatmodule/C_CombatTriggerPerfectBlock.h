#pragma once
#include "C_CombatSlotTrigger.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatTriggerPerfectBlock -- perfect-block timing window (KCD2 1.5.6, kd7u).  sizeof 0x48.
// -----------------------------------------------
// RTTI: .?AVC_CombatTriggerPerfectBlock@combatmodule@wh@@ (TD 0x184B3C2C0). Primary vtable 0x183A610A8 (22 slots).
// sub-ctor sub_180915270(this, actor) (calls SlotTrigger ctor with updateFlags=0 then registers a PB callback
// into actor+0x528 subsystem); dtor sub_1809152C4. Direct base C_CombatSlotTrigger. Adds NO own data (0x48),
// but adds 4 NEW vtable slots [18..21] over SlotTrigger's 18:
//   [18] _vf18 sub_180908BF4   [19] _vf19 sub_180909238   [20] _vf20 sub_18041A6A0(true)   [21] _vf21 sub_180909BC0
// and overrides [11] (sub_181A72CF0), [12] (sub_180909C48), OnActivate[14] (sub_180909F74),
// OnDeactivate[15] (sub_180909F4C), [16] (sub_1827CD0D8).
//
// [vs KCD1] KCD1 exposed 23 slots (added [18..22], 5 new); KCD2 has 22 (adds [18..21], 4 new) -- one
// PB-only virtual was removed. Still 0x48, no own data. OnActivate/OnDeactivate inc/dec the actor's
// perfect-block trigger count and fire the PB-window signal on the 0<->1 edges.

namespace wh::combatmodule {

class C_CombatTriggerPerfectBlock : public C_CombatSlotTrigger {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatTriggerPerfectBlock;
    // ---- 4 PB-only virtuals [18..21] (not present on Combo/Riposte) ----
    virtual void _vf18() {}     // [18] sub_180908BF4
    virtual void _vf19() {}     // [19] sub_180909238
    virtual int  _vf20() { return 1; } // [20] sub_18041A6A0 (returns true)
    virtual void _vf21() {}     // [21] sub_180909BC0
    // No own data (overrides SlotTrigger fields' meaning at runtime; layout unchanged, 0x48).
};
static_assert(sizeof(C_CombatTriggerPerfectBlock) == 0x48, "0x48, no own data (adds 4 virtuals)");

}  // namespace wh::combatmodule
