#pragma once
#include <cstdint>
#include "S_CombatWeaponCombinationDBData.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatWeaponCombinationData -- KCD2 1.5.6 (kd7u). sizeof 0x28.
// -----------------------------------------------
// combat_weapon_combination row -> Data. NON-polymorphic (no own TD). DB stack:
// C_CombatDatabase<C_ObjectDatabaseKeyIndexed<C_ObjectTableDatabase<Data, Row>,
// C_ObjectDatabaseDefaultKeyExtractor<Data>>> (OTD vtable 0x183C38318) -- i.e. this table is
// looked up BY KEY STRING, not by id (the extractor's key = the first member).
// ParseRows sub_18155CA40 converts inline: default-inits the key from a TLS-guarded static
// empty string, assigns combat_weapon_combination_key (sub_1804F3344), then copies the eight
// id columns verbatim. Non-grow push copies with `end += 40` (copy sub_18155CBF8, grow
// sub_18155C8E0) -> sizeof VERIFIED 0x28. Field order identical to the row.
//
// [vs KCD1] Table/class NEW in KCD2.

namespace wh::combatmodule {

struct C_CombatWeaponCombinationData {
    CryStringT<char> combat_weapon_combination_key;  // +0x00  lookup key ("blunt", "longsword", ...)
    int32_t          right_weapon_class_id;          // +0x08
    int32_t          left_weapon_class_id;           // +0x0C
    int32_t          right_weapon_group_id;          // +0x10
    int32_t          left_weapon_group_id;           // +0x14
    int32_t          opp_right_weapon_class_id;      // +0x18
    int32_t          opp_left_weapon_class_id;       // +0x1C
    int32_t          opp_right_weapon_group_id;      // +0x20
    int32_t          opp_left_weapon_group_id;       // +0x24
};
static_assert(sizeof(C_CombatWeaponCombinationData) == 0x28,
              "C_CombatWeaponCombinationData must be 0x28");

}  // namespace wh::combatmodule
