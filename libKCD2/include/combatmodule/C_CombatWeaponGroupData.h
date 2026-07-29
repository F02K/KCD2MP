#pragma once
#include <cstdint>
#include "I_CombatWeaponGroupData.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatWeaponGroupData : I_CombatWeaponGroupData -- KCD2 1.5.6 (kd7u). sizeof 0x28.
// -----------------------------------------------
// combat_weapon_group lookup row -> Data. POLYMORPHIC. vtable 0x183BC37B8.
// Default ctor sub_1815B18FC (RVA 0x15B18FC; id defaults -1, two empty CryStrings). Row->Data ctor
// sub_1815B1B80. DB vector-relocate loop sub_1815B1C2C strides 0x28 (v3 += 5 qwords) -> sizeof
// VERIFIED 0x28. dtor sub_1815B1CCC.
//
// [vs KCD1] KCD1 was a plain 0x20 struct (no vtable). KCD2 made it polymorphic (I_ base), which
// shifted every member up by 8 and grew it to 0x28. Field set unchanged.

namespace wh::combatmodule {

class C_CombatWeaponGroupData : public I_CombatWeaponGroupData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatWeaponGroupData;
    ~C_CombatWeaponGroupData() override;                                  // [0] 0x15B1CCC
    const CryStringT<char>& GetName() const override;                     // [1] 0x1A72EC0
    const CryStringT<char>& GetMnTag() const override;                    // [2] 0x1A72EB0
    CryStringT<char> GetSidedMnTag(int side, bool opp) const override;    // [3] 0x1827A573C

    int32_t          combat_weapon_group_id;     // +0x08  (default -1)
    int32_t          _pad0C;                      // +0x0C
    CryStringT<char> combat_weapon_group_name;    // +0x10
    CryStringT<char> mn_tag;                       // +0x18
    int32_t          combat_priority;             // +0x20
    int32_t          _pad24;                       // +0x24
};
static_assert(sizeof(C_CombatWeaponGroupData) == 0x28, "C_CombatWeaponGroupData must be 0x28");

}  // namespace wh::combatmodule
