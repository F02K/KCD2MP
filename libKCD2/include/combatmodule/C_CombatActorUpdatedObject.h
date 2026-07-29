#pragma once
#include <cstdint>
#include "C_CombatActorObject.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorUpdatedObject : C_CombatActorObject -- KCD2 1.5.6 (kd7u).  sizeof 0x18.
// -----------------------------------------------
// Adds the per-frame update registration. ctor sub_180914754(this, actor, updateFlags): stores the
// flags at +0x10 and, when non-zero, registers into the actor's update lists via sub_180914834 --
// bits 1/2/4/8 each select one of the owner's four update-phase lists (so 4 = phase 3 only,
// 9 = phases 1+4). Observed: 4 = OpponentManager, 9 = Automation.

namespace wh::combatmodule {

class C_CombatActorUpdatedObject : public C_CombatActorObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorUpdatedObject;
    // Four per-phase tick callbacks [6..9], one per m_updateFlags bit (mapping = inference from the
    // four registration lists in sub_180914834; all resolve to no-op stubs in the automation vtables).
    virtual void UpdatePhase1() {}       // [6]
    virtual void UpdatePhase2() {}       // [7]
    virtual void UpdatePhase3() {}       // [8]
    virtual void UpdatePhase4() {}       // [9]

    int32_t  m_updateFlags;              // +0x10  update-phase bitmask (bits 1/2/4/8); non-zero = registered
    uint32_t _pad14;                     // +0x14
};
static_assert(sizeof(C_CombatActorUpdatedObject) == 0x18, "C_CombatActorUpdatedObject must be 0x18");

}  // namespace wh::combatmodule
