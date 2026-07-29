#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatZoneData -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Pure interface base of C_CombatZoneData (subobject @+0x00, vtable-only, size 0x08). KCD2 TD
// exists but has no standalone vtable instance (abstract; realized through the C_ vtable
// 0x183A83D70). Slots: [0] dtor, [1..4] pure; C_ overrides all five. The four accessors each
// return one of the zone's mn-tag CryStrings (by reference); per-tag roles resolved from the
// combat_zone column binder sub_1800C7520 (see C_CombatZoneData.h / data_verticals_2.md).

namespace wh::combatmodule {

class I_CombatZoneData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatZoneData;
    virtual ~I_CombatZoneData() = default;                                 // [0]
    virtual const CryStringT<char>& GetAttackMnTag() const = 0;            // [1] C_: 0x1A72EB0 -> C_+0x18
    virtual const CryStringT<char>& GetDefenseMnTag() const = 0;           // [2] C_: 0x1A73160 -> C_+0x20
    virtual const CryStringT<char>& GetStartMnTag() const = 0;             // [3] C_: 0x1A749E0 -> C_+0x30 (start_mn_tag)
    virtual const CryStringT<char>& GetEndMnTag() const = 0;               // [4] C_: 0x1A73520 -> C_+0x40 (end_mn_tag)
};

}  // namespace wh::combatmodule
