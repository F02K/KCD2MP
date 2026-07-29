#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatDamageTypeMappingData -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// combat_damage_type_mapping row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_ObjectTableDatabase<C_CombatDamageTypeMappingData, S_CombatDamageTypeMappingDBData>> =
// C_CombatDamageTypeMappingDatabase (vtable 0x183C39CD0, 22 slots). ParseRows (vtable[10] =
// sub_1815DD2A4, COMDAT-folded with C_CombatAttackHitStatisticsData's parse) copies each 0x10
// raw row VERBATIM (4 dwords); the in-place push sub_182420E30 advances the vector end by 16 ->
// sizeof PROVEN 0x10. Field order = S_CombatDamageTypeMappingDBData (descriptor-init
// sub_1800C0630).
//
// [vs KCD1] combat_damage_type_mapping does not exist in the KCD1 table-schema extraction
// (table_schemas_v2.json) -- this vertical is new in KCD2.

namespace wh::combatmodule {

class C_CombatDamageTypeMappingData {
public:
    int32_t combat_attack_type_id;   // +0x00  PK, FK -> combat_attack_type
    int32_t rpg_damage_type_id;      // +0x04  PK, FK -> rpg damage type
    int32_t r_weapon_class_id;       // +0x08  PK, FK -> weapon_class (right hand)
    int32_t r_weapon_sub_class_id;   // +0x0C  PK, FK -> weapon_sub_class (right hand)
};
static_assert(sizeof(C_CombatDamageTypeMappingData) == 0x10,
              "C_CombatDamageTypeMappingData must be 0x10 (push helper sub_182420E30 advances 16)");

}  // namespace wh::combatmodule
