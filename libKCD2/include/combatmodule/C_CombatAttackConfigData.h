#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatAttackConfigData -- KCD2 1.5.6 (kd7u). sizeof 0x1C.
// -----------------------------------------------
// combat_attack_config row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_AnimationDatabaseStatic<C_CombatAttackConfigData, S_CombatAttackConfigTableRow>> =
// C_CombatAttackConfigDatabase (vtable 0x183C3F958, 23 slots). ParseRows (vtable[10] =
// sub_180F6DEDC) copies each 0x1C raw row VERBATIM (7 dwords); the in-place push sub_182799640
// advances the vector end by 28 -> sizeof PROVEN 0x1C. Field order = S_CombatAttackConfigTableRow
// (descriptor-init sub_1800C2350).
//
// [vs KCD1] No KCD1 reference header; the KCD1 table schema (combat_attack_config, rowsize 0x1C,
// same 7 columns) matches -- layout unchanged.

namespace wh::combatmodule {

class C_CombatAttackConfigData {
public:
    int32_t combat_attack_config_id;    // +0x00  PK
    int32_t combat_zone_id;             // +0x04  FK -> combat_zone
    int32_t combat_input_class_id;      // +0x08  FK -> combat_input_class
    int32_t r_weapon_class_id;          // +0x0C  FK -> weapon_class (right hand)
    int32_t l_weapon_class_id;          // +0x10  FK -> weapon_class (left hand)
    int32_t r_combat_weapon_group_id;   // +0x14  FK -> combat_weapon_group (default -1)
    int32_t l_combat_weapon_group_id;   // +0x18  FK -> combat_weapon_group (default -1)
};
static_assert(sizeof(C_CombatAttackConfigData) == 0x1C,
              "C_CombatAttackConfigData must be 0x1C (push helper sub_182799640 advances 28)");

}  // namespace wh::combatmodule
