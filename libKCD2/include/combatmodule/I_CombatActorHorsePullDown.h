#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatActorHorsePullDown -- horse-pulldown dispatch interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI: .?AVI_CombatActorHorsePullDown@combatmodule@wh@@ (KCD2 TD present). Secondary MI base of
// C_CombatActorHorsePullDown; its 3-slot vtable sits at object +0x18 (0x183A62DB0). The
// "victim-targeted action" shape shared with I_CombatActorHuntAttack / I_CombatActorMercyKill:
//   [0] ~dtor  [1] Can (tri-state verdict)  [2] Request (dispatch).
// Impls reach the owning C_CombatActor via (this - 0x10) == object+0x08 (C_CombatActorObject::m_pOwner).
//
// CanHorsePullDown (sub_18072FA9C) verdict: 0 = blocked (target invalid / not mounted / range / facing-angle
//   fail), 1 = eligible, 2 = approved (relationship + RPG roll type 11 passed). RequestHorsePullDown
//   (sub_18275E848) first re-checks Can()==2 (early-outs otherwise), then dispatches the synced dehorse.

namespace wh::combatmodule {

class I_CombatActorHorsePullDown {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActorHorsePullDown;
    virtual ~I_CombatActorHorsePullDown() = default;                 // [0]
    virtual int  CanHorsePullDown(uint32_t victimEntityId) = 0;      // [1] sub_18072FA9C (tri-state 0/1/2)
    virtual bool RequestHorsePullDown(uint32_t victimEntityId) = 0;  // [2] sub_18275E848 (requires Can()==2)
};

}  // namespace wh::combatmodule
