#pragma once
#include <cstddef>
#include <cstdint>
#include "C_CombatActorObject.h"
#include "C_WeaponProcSolver.h"
#include "C_WeaponSolver.h"
#include "S_CombatCollisionDetails.h"
#include "../animationmodule/I_SceneListener.h"
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorCollisions : C_CombatActorObject,
//                                             wh::animationmodule::I_SceneListener
// (KCD2 WHGame.dll 1.5.6). sizeof 0x268.
// -----------------------------------------------
// RTTI pins the bases at +0x00/+0x10. Ctor 0x181481B9C owns both weapon solvers and initializes
// two S_CombatCollisionDetails records at +0x98/+0x170. KCD1 signal RTTI plus matched KCD2 emit
// paths establish the two signal signatures at +0x18/+0x28.

namespace wh::combatmodule {

class C_CombatActorCollisions : public C_CombatActorObject,
                                public wh::animationmodule::I_SceneListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorCollisions;
    ~C_CombatActorCollisions() override = default;
    void Reset() override;                                                     // 0x18277F9E8
    E_CombatSubsystem GetSubsystemId() const override { return static_cast<E_CombatSubsystem>(15); }
    const char* GetName() const override { return ""; }

    void OnCollision(const void* participant0,
                     const void* participant1,
                     const EventPhysCollision& collision) override;            // 0x180D51854

    wh::shared::C_Signal<const S_CombatCollisionDetails&, bool&> m_collisionFilter; // +0x18
    wh::shared::C_Signal<const S_CombatCollisionDetails&> m_onCollision;        // +0x28
    uint8_t                   m_unk38[0x50];                                    // +0x38
    C_WeaponProcSolver*       m_pWeaponProcSolver;                              // +0x88 owned
    C_WeaponSolver*           m_pWeaponSolver;                                  // +0x90 owned
    S_CombatCollisionDetails  m_collisionDetails[2];                            // +0x98
    uint8_t                   m_unk248[0x20];                                   // +0x248
};
static_assert(sizeof(C_CombatActorCollisions) == 0x268,
              "C_CombatActorCollisions must be 0x268");
static_assert(offsetof(C_CombatActorCollisions, m_collisionFilter) == 0x18);
static_assert(offsetof(C_CombatActorCollisions, m_pWeaponProcSolver) == 0x88);
static_assert(offsetof(C_CombatActorCollisions, m_collisionDetails) == 0x98);
static_assert(offsetof(C_CombatActorCollisions, m_unk248) == 0x248);

}  // namespace wh::combatmodule
