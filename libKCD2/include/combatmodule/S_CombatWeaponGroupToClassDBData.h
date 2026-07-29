#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatWeaponGroupToClassDBData -- KCD2 1.5.6 (kd7u). sizeof 0x0C.
// -----------------------------------------------
// Raw combat_weapon_group_to_class table row (Libs/Tables/combat/combat_weapon_group_to_class
// .xml) -- the weapon_group <-> weapon_class many-to-many join. Row type of
// C_ObjectTableDatabase<C_CombatWeaponGroupToClassData, S_CombatWeaponGroupToClassDBData>
// (vtable 0x183C2BE88). ParseRows sub_18160C5D0 walks the raw buffer with stride 0x0C
// (`rawBase + 12*i`) -> sizeof VERIFIED 0x0C. Columns from the binder sub_1800C3B40:
// two PK int columns + the bool column only_for_one_hand @+0x08 (descriptor built by the
// helper sub_1819DAA00: type 4 = bool, size 1, default false).
//
// [vs KCD1] only_for_one_hand is NEW in KCD2 (KCD1 had just the two id columns); XML omits it
// on rows where it keeps the default.

namespace wh::combatmodule {

struct S_CombatWeaponGroupToClassDBData {
    int32_t combat_weapon_group_id;  // +0x00  PK
    int32_t weapon_class_id;         // +0x04  PK
    bool    only_for_one_hand;       // +0x08  default false
    uint8_t _pad09[3];               // +0x09
};
static_assert(sizeof(S_CombatWeaponGroupToClassDBData) == 0x0C,
              "S_CombatWeaponGroupToClassDBData must be 0x0C");

}  // namespace wh::combatmodule
