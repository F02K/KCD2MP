#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatDamageTypeMappingDBData -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// Raw combat_damage_type_mapping table row (Libs/Tables/combat/combat_damage_type_mapping.xml):
// maps (attack type, right-hand weapon class/subclass) to the rpg damage type dealt. Row type of
// C_ObjectTableDatabase<C_CombatDamageTypeMappingData, S_CombatDamageTypeMappingDBData> under
// C_CombatDamageTypeMappingDatabase. Register (vtable[7] = sub_1819422C4) binds the 4-descriptor
// array qword_1854A67C0 with row stride 16 -> sizeof PROVEN 0x10. ALL FOUR columns are int32
// C_PKColumnDescriptor entries (composite key; static-init sub_1800C0630).

namespace wh::combatmodule {

struct S_CombatDamageTypeMappingDBData {
    int32_t combat_attack_type_id;   // +0x00  PK, FK -> combat_attack_type
    int32_t rpg_damage_type_id;      // +0x04  PK, FK -> rpg damage type
    int32_t r_weapon_class_id;       // +0x08  PK, FK -> weapon_class (right hand)
    int32_t r_weapon_sub_class_id;   // +0x0C  PK, FK -> weapon_sub_class (right hand)
};
static_assert(sizeof(S_CombatDamageTypeMappingDBData) == 0x10,
              "S_CombatDamageTypeMappingDBData must be 0x10 (registered row stride 16)");

}  // namespace wh::combatmodule
