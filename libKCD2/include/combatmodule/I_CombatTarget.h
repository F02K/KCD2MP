#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatTarget -- uniform handle to a combat target
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable only).
// -----------------------------------------------
// RTTI .?AVI_CombatTarget@combatmodule@wh@@   abstract vtable 0x183A79738
//   [0] deleting dtor (sub_18275CD88)   [1..7] purecall (sub_181D93E5D)   [8] = COL boundary.
// So the interface is dtor + 7 pure methods -- shape-identical to KCD1 (8 slots). The sole concrete
// implementation, C_CombatTarget, wraps a CryEngine IEntity* and forwards every query to it. wh::
// classes are not interfuscator-shuffled, so slot order matches KCD1; slot [1] (GetActor) was
// re-verified in kd7u (sub_180947210 = "return *(this+8)"). Slots [2..7] are ported with their KCD1
// semantics [impl-correlated: shape verified, per-method bodies not re-decompiled in KCD2].

namespace Offsets { struct IEntity; }

namespace wh::combatmodule {

class C_CombatActor;

class I_CombatTarget {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatTarget;
    virtual ~I_CombatTarget() = default;                            // [0]
    virtual Offsets::IEntity* GetActor() const = 0;                 // [1]  sub_180947210 -> *(this+8)
    virtual uint32_t GetEntityId() const = 0;                       // [2]  wrapped entity's id
    virtual Vec3 GetPosition() const = 0;                           // [3]  entity world position
    virtual Quat GetWorldRotation() const = 0;                      // [4]  entity world rotation (Quat)
    virtual void* LookupEntity() const = 0;                         // [5]  resolve entity -> actor/soul
    virtual C_CombatActor* GetCombatActor() const = 0;              // [6]  wrapped entity's combat actor
    virtual bool IsSameTarget(const I_CombatTarget* other) const = 0; // [7]  GetActor() identity
};

}  // namespace wh::combatmodule
