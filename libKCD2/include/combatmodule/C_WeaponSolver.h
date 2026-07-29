#pragma once
#include <cstddef>
#include <cstdint>
#include "C_CombatActorObject.h"

// -----------------------------------------------
// wh::combatmodule::C_WeaponSolver : C_CombatActorObject
// (KCD2 WHGame.dll 1.5.6). sizeof 0x50.
// -----------------------------------------------
// RTTI verifies the sole base. C_CombatActorCollisions owns an instance allocated as 0x50.
// Ctor is 0x181481D80; subsystem id 14; diagnostic name "WeaponSolver2". Internal solver state is
// intentionally left opaque until its individual members are independently verified.

namespace wh::combatmodule {

class C_WeaponSolver : public C_CombatActorObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeaponSolver;
    ~C_WeaponSolver() override = default;
    E_CombatSubsystem GetSubsystemId() const override { return static_cast<E_CombatSubsystem>(14); }
    const char* GetName() const override { return "WeaponSolver2"; }

    uint8_t m_unk10[0x40];
};
static_assert(sizeof(C_WeaponSolver) == 0x50, "C_WeaponSolver must be 0x50");
static_assert(offsetof(C_WeaponSolver, m_unk10) == 0x10);

}  // namespace wh::combatmodule
