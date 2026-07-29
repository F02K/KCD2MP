#pragma once
#include <cstdint>
#include "I_CombatTarget.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatTarget -- concrete I_CombatTarget wrapping a CryEngine IEntity*
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI .?AVC_CombatTarget@combatmodule@wh@@   vtable 0x183A796F0 (8 slots; [8] = COL boundary).
//   [0] sub_180B217A8 deleting dtor   [1] sub_180947210 GetActor -> m_pActor   [2] sub_180ABC730
//   [3] sub_181402370   [4] sub_18275E1F0   [5] sub_18072DC18   [6] sub_18072DB98   [7] sub_180B217D4
// Shape-identical to KCD1. Frequently stack-constructed inline (e.g. sub_180B21384 builds a 0x10
// C_CombatTarget on the stack: vtable @+0, entity @+0x08), which pins sizeof 0x10 and m_pActor@+0x08.

namespace Offsets { struct IEntity; }

namespace wh::combatmodule {

class C_CombatTarget : public I_CombatTarget {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatTarget;
    ~C_CombatTarget() override = default;                                    // [0] sub_180B217A8
    Offsets::IEntity* GetActor() const override { return m_pActor; }         // [1] sub_180947210
    uint32_t GetEntityId() const override { return 0; }                      // [2] sub_180ABC730
    Vec3 GetPosition() const override { return {}; }                         // [3] sub_181402370
    Quat GetWorldRotation() const override { return {}; }                    // [4] sub_18275E1F0
    void* LookupEntity() const override { return nullptr; }                  // [5] sub_18072DC18
    C_CombatActor* GetCombatActor() const override { return nullptr; }       // [6] sub_18072DB98
    bool IsSameTarget(const I_CombatTarget* other) const override            // [7] sub_180B217D4
    { return false; }

    Offsets::IEntity* m_pActor;     // +0x08  wrapped target entity
};
static_assert(sizeof(C_CombatTarget) == 0x10, "C_CombatTarget must be 0x10");

}  // namespace wh::combatmodule
