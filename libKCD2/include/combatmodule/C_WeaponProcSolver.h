#pragma once
#include <cstddef>
#include <cstdint>
#include "C_CombatActorObject.h"

// -----------------------------------------------
// wh::combatmodule::C_WeaponProcSolver : C_CombatActorObject
// (KCD2 WHGame.dll 1.5.6). sizeof 0x3C8.
// -----------------------------------------------
// RTTI verifies the sole base. C_CombatActorCollisions owns an instance allocated as 0x3C8.
// Ctor is 0x181481E18; subsystem id 13; diagnostic name "WeaponSolver". Internal solver state is
// intentionally left opaque until its individual members are independently verified.

namespace wh::combatmodule {

class C_WeaponProcSolver : public C_CombatActorObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeaponProcSolver;
    ~C_WeaponProcSolver() override = default;
    E_CombatSubsystem GetSubsystemId() const override { return static_cast<E_CombatSubsystem>(13); }
    const char* GetName() const override { return "WeaponSolver"; }

    uint8_t m_unk10[0x3B8];
};
static_assert(sizeof(C_WeaponProcSolver) == 0x3C8, "C_WeaponProcSolver must be 0x3C8");
static_assert(offsetof(C_WeaponProcSolver, m_unk10) == 0x10);

}  // namespace wh::combatmodule
