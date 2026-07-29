#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatAttackConfigTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x1C.
// -----------------------------------------------
// Raw combat_attack_config table row (Libs/Tables/combat/combat_attack_config.xml): which
// zone/input-class/weapon combinations form a valid attack. Row type of
// C_AnimationDatabaseStatic<C_CombatAttackConfigData, S_CombatAttackConfigTableRow> under
// C_CombatAttackConfigDatabase. Register (vtable[7] = sub_1819425A0) binds the 7-descriptor
// array qword_1854A6B20 with row stride 28 -> sizeof PROVEN 0x1C. Columns from descriptor
// static-init sub_1800C2350; the two *_combat_weapon_group_id columns carry an explicit
// default of -1 (descriptor has-default word 257, default cell -1).

namespace wh::combatmodule {

struct S_CombatAttackConfigTableRow {
    int32_t combat_attack_config_id;    // +0x00  PK
    int32_t combat_zone_id;             // +0x04  FK -> combat_zone
    int32_t combat_input_class_id;      // +0x08  FK -> combat_input_class
    int32_t r_weapon_class_id;          // +0x0C  FK -> weapon_class (right hand)
    int32_t l_weapon_class_id;          // +0x10  FK -> weapon_class (left hand)
    int32_t r_combat_weapon_group_id;   // +0x14  FK -> combat_weapon_group (default -1)
    int32_t l_combat_weapon_group_id;   // +0x18  FK -> combat_weapon_group (default -1)
};
static_assert(sizeof(S_CombatAttackConfigTableRow) == 0x1C,
              "S_CombatAttackConfigTableRow must be 0x1C (registered row stride 28)");

}  // namespace wh::combatmodule
