#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatAttackTypeTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x30.
// -----------------------------------------------
// Raw combat_attack_type table row (Libs/Tables/combat/combat_attack_type.xml). Row type of
// C_ObjectTableDatabase<C_CombatAttackTypeData, S_CombatAttackTypeTableRow> (vtable
// 0x183ABE098). Row accessor sub_1814FBCF8 returns `rawBase + 48*i` -> sizeof VERIFIED 0x30.
// Column names/offsets/types from the binder sub_1800C2770 (+ the cannot_kill descriptor
// helper sub_1819DAA58: type 4 = bool, offset 40, default false).
// Row->Data converter sub_1814FBD10 maps: +0x00->Data+0x08, +0x08->Data+0x10 (CryString),
// +0x10->Data+0x18, +0x14->Data+0x1C, +0x18->Data+0x20, +0x20->Data+0x40, +0x28->Data+0x5C
// (see C_CombatAttackTypeData.h; the Data's runtime tag blocks are zero-initialized, not
// row-fed).

namespace wh::combatmodule {

struct S_CombatAttackTypeTableRow {
    int32_t     combat_attack_type_id;    // +0x00  PK
    int32_t     _pad04;                   // +0x04
    const char* combat_attack_type_name;  // +0x08
    int32_t     rpg_damage_type_id;       // +0x10
    float       relative_stamina_cost;    // +0x14
    const char* mn_tag;                   // +0x18
    const char* opp_mn_tag;               // +0x20
    bool        cannot_kill;              // +0x28  default false
    uint8_t     _pad29[7];                // +0x29
};
static_assert(sizeof(S_CombatAttackTypeTableRow) == 0x30,
              "S_CombatAttackTypeTableRow must be 0x30");

}  // namespace wh::combatmodule
