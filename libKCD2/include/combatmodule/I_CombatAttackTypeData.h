#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatAttackTypeData -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Pure interface base of C_CombatAttackTypeData (subobject @+0x00, vtable-only, size 0x08).
// I_ vtable 0x183BA65F0: [0] base dtor sub_18275CD5C (RVA 0x275CD5C), [1..3] pure (_purecall);
// C_CombatAttackTypeData overrides all four. Accessor semantics recovered from the concrete
// overrides (offsets are into the C_ object).

namespace wh::combatmodule {

class I_CombatAttackTypeData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatAttackTypeData;
    virtual ~I_CombatAttackTypeData() = default;             // [0] 0x275CD5C
    virtual float   GetRelativeStaminaCost() const = 0;      // [1] C_: 0x1A7E030 -> C_+0x1C
    virtual int32_t GetRpgDamageTypeId() const = 0;          // [2] C_: 0x1A72850 -> C_+0x18
    virtual bool    GetCannotKill() const = 0;               // [3] C_: 0x1350EE0 -> C_+0x5C
};

}  // namespace wh::combatmodule
