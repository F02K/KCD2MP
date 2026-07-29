#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatWeaponGroupData -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Pure interface base of C_CombatWeaponGroupData (subobject @+0x00, vtable-only, size 0x08).
// KCD2 TD exists but has no standalone vtable instance (abstract; only realized through the C_
// vtable 0x183BC37B8). Slots: [0] dtor, [1..3] pure; C_ overrides all. Accessor semantics from
// the concrete overrides (offsets into the C_ object).

namespace wh::combatmodule {

class I_CombatWeaponGroupData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatWeaponGroupData;
    virtual ~I_CombatWeaponGroupData() = default;                         // [0]
    virtual const CryStringT<char>& GetName() const = 0;                  // [1] C_: 0x1A72EC0 -> C_+0x10
    virtual const CryStringT<char>& GetMnTag() const = 0;                 // [2] C_: 0x1A72EB0 -> C_+0x18
    // Builds a per-hand "%c_%s" mn-tag ("l_"/"r_" + mn_tag, optionally "o_"-wrapped for opponent).
    virtual CryStringT<char> GetSidedMnTag(int side, bool opp) const = 0; // [3] C_: 0x1827A573C
};

}  // namespace wh::combatmodule
