#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatActorObject -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10.
// -----------------------------------------------
// Base class of every combat-actor subsystem object. ctor sub_180914798(this, actor): stores the
// owner and registers this subsystem on the owning actor (sub_1809147C8). 6-slot vtable, roles
// recovered from the C_CombatActorHuntAttack override set (Reset sub_181A7E060 clears state;
// GetSubsystemId sub_181A7DF40 returns the E_CombatSubsystem constant; GetName sub_181A7DE50;
// [4]/[5] build the "%s: %s [%s]" debug label / description -- their canonical names tentative).

namespace wh::combatmodule {

class C_CombatActor;
enum E_CombatSubsystem : int32_t;   // subsystem ids (KCD2 values: 3=ActionManager, 25=Automation, 29=HuntAttack, ...)

class C_CombatActorObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorObject;
    virtual ~C_CombatActorObject() = default;                    // [0]
    virtual void Reset() {}                                      // [1]  subsystem state reset
    virtual E_CombatSubsystem GetSubsystemId() const = 0;        // [2]
    virtual const char* GetName() const = 0;                     // [3]
    virtual void GetDebugLabel(void* outLabel) {}                // [4]  (name tentative)
    virtual void GetDebugInfo(void* outInfo) {}                  // [5]  (name tentative)

    C_CombatActor* m_pOwner;                                     // +0x08
};
static_assert(sizeof(C_CombatActorObject) == 0x10, "C_CombatActorObject must be 0x10");

}  // namespace wh::combatmodule
