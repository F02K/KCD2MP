#pragma once
#include <cstdint>
#include "C_CombatActorObject.h"
#include "I_CombatActorHuntAttack.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorHuntAttack -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// ctor sub_180917694. MI: C_CombatActorObject (primary, vtable 0x183A62DF0: Reset sub_181A7E060
// zeroes the victim id; GetSubsystemId -> 29 (HUNT_ATTACK); GetName -> "HUNT_ATTACK") +
// I_CombatActorHuntAttack (secondary vtable @+0x10). No signals, no manager pointers.

namespace wh::combatmodule {

class C_CombatActorHuntAttack : public C_CombatActorObject, public I_CombatActorHuntAttack {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorHuntAttack;
    uint32_t m_victimEntityId;   // +0x18  EntityId latched by Request; cleared by Reset
    uint32_t _pad1C;             // +0x1C  tail alignment (never touched)
};
static_assert(sizeof(C_CombatActorHuntAttack) == 0x20, "C_CombatActorHuntAttack must be 0x20");

}  // namespace wh::combatmodule
