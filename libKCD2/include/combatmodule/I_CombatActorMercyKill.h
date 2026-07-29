#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatActorMercyKill -- mercy-kill dispatch interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI: .?AVI_CombatActorMercyKill@combatmodule@wh@@ (KCD2 TD present). Secondary MI base of
// C_CombatActorMercyKill; its 3-slot vtable sits at object +0x10 (0x183A63128). Same
// "victim-targeted action" shape as I_CombatActorHuntAttack / I_CombatActorHorsePullDown:
//   [0] ~dtor  [1] Can (tri-state)  [2] Request (dispatch).
// Impls reach the owning C_CombatActor via (this - 0x08) == object+0x08 (C_CombatActorObject::m_pOwner).
//
// CanMercyKill (sub_1816043EC) verdict: 0 = blocked (target invalid / not downed / no action template),
//   1 = eligible, 2 = approved (relationship + RPG roll type 11 passed). RequestMercyKill (sub_18275ED74)
//   does NOT re-check eligibility -- it dispatches directly (unlike HuntAttack/HorsePullDown which need ==2).

namespace wh::combatmodule {

class I_CombatActorMercyKill {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActorMercyKill;
    virtual ~I_CombatActorMercyKill() = default;                 // [0]
    virtual int  CanMercyKill(uint32_t victimEntityId) = 0;      // [1] sub_1816043EC (tri-state 0/1/2)
    virtual bool RequestMercyKill(uint32_t victimEntityId) = 0;  // [2] sub_18275ED74 (dispatches unconditionally)
};

}  // namespace wh::combatmodule
