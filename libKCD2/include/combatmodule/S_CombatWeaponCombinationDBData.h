#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatWeaponCombinationDBData -- KCD2 1.5.6 (kd7u). sizeof 0x28.
// -----------------------------------------------
// Raw combat_weapon_combination table row (Libs/Tables/combat/combat_weapon_combination.xml).
// Row type of C_ObjectTableDatabase<C_CombatWeaponCombinationData,
// S_CombatWeaponCombinationDBData> (vtable 0x183C38318). ParseRows sub_18155CA40 walks the raw
// buffer with stride 0x28 (`rawBase + 40*i`) -> sizeof VERIFIED 0x28. Column names/offsets from
// the binder sub_1800C3770; the key is the only string (type 5, @+0x00, NOT a PK descriptor);
// the eight id columns are built by the int-descriptor helper sub_180D16898 (weapon_class ids
// with default blob unk_183E59E68, weapon_group ids with unk_183E5A0D0 -- defaults, XML omits
// unset columns).
//
// [vs KCD1] Table is NEW in KCD2 (no KCD1 row registration / header exists).

namespace wh::combatmodule {

struct S_CombatWeaponCombinationDBData {
    const char* combat_weapon_combination_key;  // +0x00  unique key string, e.g. "longsword"
    int32_t     right_weapon_class_id;          // +0x08
    int32_t     left_weapon_class_id;           // +0x0C
    int32_t     right_weapon_group_id;          // +0x10
    int32_t     left_weapon_group_id;           // +0x14
    int32_t     opp_right_weapon_class_id;      // +0x18
    int32_t     opp_left_weapon_class_id;       // +0x1C
    int32_t     opp_right_weapon_group_id;      // +0x20
    int32_t     opp_left_weapon_group_id;       // +0x24
};
static_assert(sizeof(S_CombatWeaponCombinationDBData) == 0x28,
              "S_CombatWeaponCombinationDBData must be 0x28");

}  // namespace wh::combatmodule
