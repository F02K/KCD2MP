#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatZoneConfigTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x24.
// -----------------------------------------------
// Raw combat_zone_config table row (Libs/Tables/combat/combat_zone_config.xml) -- per
// weapon-loadout guard-zone weighting. Row type of
// C_ObjectTableDatabase<C_CombatZoneConfigData, S_CombatZoneConfigTableRow> (vtable
// 0x183C3F370). ParseRows sub_1815C6658 walks the raw buffer with stride 0x24
// (`rawBase + 36*i`) -> sizeof VERIFIED 0x24. Columns from the binder sub_1800C3C40
// (+ the injured_mod descriptor helper sub_1819DA9AC: type 2 = float, offset 32,
// default 1.0f). r/l_combat_weapon_group_id default to -1 (flag 0x101).
//
// [vs KCD1] injured_mod is NEW in KCD2 (KCD1 row was 0x20 and ended at guard_weight);
// KCD1's 4-byte row header is gone (0-based offsets).

namespace wh::combatmodule {

struct S_CombatZoneConfigTableRow {
    int32_t combat_zone_config_id;     // +0x00  PK
    int32_t combat_zone_id;            // +0x04
    int32_t guard_stance_id;           // +0x08
    int32_t r_weapon_class_id;         // +0x0C
    int32_t l_weapon_class_id;         // +0x10
    int32_t r_combat_weapon_group_id;  // +0x14  default -1
    int32_t l_combat_weapon_group_id;  // +0x18  default -1
    float   guard_weight;              // +0x1C
    float   injured_mod;               // +0x20  default 1.0f
};
static_assert(sizeof(S_CombatZoneConfigTableRow) == 0x24,
              "S_CombatZoneConfigTableRow must be 0x24");

}  // namespace wh::combatmodule
