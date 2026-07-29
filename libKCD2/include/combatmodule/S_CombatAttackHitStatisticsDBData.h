#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatAttackHitStatisticsDBData -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// Raw combat_attack_hit_statistics table row (Libs/Tables/combat/combat_attack_hit_statistics.xml):
// per (attack type, zone, body subpart) hit counters used as hit-distribution weights. Row type of
// C_ObjectTableDatabase<C_CombatAttackHitStatisticsData, S_CombatAttackHitStatisticsDBData> under
// C_CombatAttackHitStatisticsDatabase. Register (vtable[7] = sub_1819424AC) binds the 4-descriptor
// array qword_1854A6B58 with row stride 16 -> sizeof PROVEN 0x10. Columns from descriptor
// static-init sub_1800C25F0: the first THREE are C_PKColumnDescriptor entries (composite key),
// hit_count is a plain int32.

namespace wh::combatmodule {

struct S_CombatAttackHitStatisticsDBData {
    int32_t combat_attack_type_id;  // +0x00  PK, FK -> combat_attack_type
    int32_t combat_zone_id;         // +0x04  PK, FK -> combat_zone
    int32_t body_subpart_id;        // +0x08  PK, FK -> body_subpart
    int32_t hit_count;              // +0x0C  recorded hit tally
};
static_assert(sizeof(S_CombatAttackHitStatisticsDBData) == 0x10,
              "S_CombatAttackHitStatisticsDBData must be 0x10 (registered row stride 16)");

}  // namespace wh::combatmodule
