#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatActorHuntAttack -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Secondary MI base of C_CombatActorHuntAttack (vtable 0x183A62DD0, 3 slots). TryHuntAttack runs
// the speed/distance/angle/RPG gate checks (wh_cs_HuntAttack* params) and returns the verdict;
// Request re-checks then dispatches the sync attack and latches the victim id. Verdict values 0/2
// observed (KCD1 semantics: 0 = Blocked, 1 = Denied, 2 = Approved; the 1 case was not observed in
// the KCD2 decompile).

namespace wh::combatmodule {

class I_CombatActorHuntAttack {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActorHuntAttack;
    virtual ~I_CombatActorHuntAttack() = default;            // [0] (thunk to primary dtor)
    virtual int  TryHuntAttack(uint32_t victimEntityId) = 0; // [1] sub_180A512B0
    virtual void Request(uint32_t victimEntityId) = 0;       // [2] sub_18275EB64
};

}  // namespace wh::combatmodule
