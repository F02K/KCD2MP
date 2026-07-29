#pragma once
#include <cstdint>
#include "S_CombatWeaponGroupToClassDBData.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatWeaponGroupToClassData -- KCD2 1.5.6 (kd7u). sizeof 0x0C.
// -----------------------------------------------
// combat_weapon_group_to_class row -> Data. NON-polymorphic POD, layout IDENTICAL to
// S_CombatWeaponGroupToClassDBData (no FK resolution, no strings). DB stack: plain
// C_CombatDatabase<C_ObjectTableDatabase<Data, Row>> (vtable 0x183C2BE88).
// ParseRows sub_18160C5D0 copies dword/dword/byte verbatim into a stack temp and pushes it;
// push helper sub_182420E78 advances the vector end by 12 -> sizeof VERIFIED 0x0C
// (grow path sub_18098A0E4).
//
// [vs KCD1] only_for_one_hand column is NEW in KCD2.

namespace wh::combatmodule {

struct C_CombatWeaponGroupToClassData {
    int32_t combat_weapon_group_id;  // +0x00
    int32_t weapon_class_id;         // +0x04
    bool    only_for_one_hand;       // +0x08
    uint8_t _pad09[3];               // +0x09
};
static_assert(sizeof(C_CombatWeaponGroupToClassData) == 0x0C,
              "C_CombatWeaponGroupToClassData must be 0x0C");

}  // namespace wh::combatmodule
