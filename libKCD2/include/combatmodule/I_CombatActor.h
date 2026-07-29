#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatActor -- pure combat-actor interface (KCD2 1.5.6, kd7u)
// -----------------------------------------------
// Primary base of C_CombatActor (subobject at +0x00, size 0x08 = vtable only).
// Implemented by C_CombatActor.
//
// The kd7u C_CombatActor primary vtable is 217 slots; the exact I_CombatActor /
// C_CombatActor boundary is NOT separately recovered, so only the verified
// interface slots through GetEntityId[8] are declared here. Slots [9..216] belong
// to the concrete class and are documented in C_CombatActor.h. Where the boundary
// falls does not affect object layout (the object stores only vtable pointers);
// declaring the interface methods in their real slot order keeps GetEntity/
// GetEntityId callable at the correct index.
//
// [vs KCD1] The interface widened by one slot at the front: GetEntity moved
// [6]->[7] and GetEntityId [7]->[8].

namespace Offsets { struct IEntity; }

namespace wh::combatmodule {

class I_CombatActor {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActor;
    virtual ~I_CombatActor() = default;                 // [0]
    virtual void unk_1() = 0;                           // [1]
    virtual void unk_2() = 0;                           // [2]
    virtual void unk_3() = 0;                           // [3]
    virtual void unk_4() = 0;                           // [4]
    virtual void unk_5() = 0;                           // [5]
    virtual void unk_6() = 0;                           // [6]
    virtual Offsets::IEntity* GetEntity() const = 0;    // [7]  0x181A739B0  returns *(this+0x1F8)  VERIFIED
    virtual uint32_t GetEntityId() const = 0;           // [8]  0x180C57690  returns *(m_pEntity+0x30) VERIFIED
};

}  // namespace wh::combatmodule
