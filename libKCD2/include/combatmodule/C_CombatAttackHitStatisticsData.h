#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatAttackHitStatisticsData -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// combat_attack_hit_statistics row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_ObjectDatabaseSortedIdIndexed<C_ObjectTableDatabase<C_CombatAttackHitStatisticsData,
// S_CombatAttackHitStatisticsDBData>, std::pair<int,int>>> = C_CombatAttackHitStatisticsDatabase
// (vtable 0x183C375B0, 22 slots; MI subobject @+0xB0, vtable 0x183C37410 -- the sorted-index
// layer keyed by std::pair<int,int>). ParseRows (vtable[10] = sub_1815DD2A4, COMDAT-folded with
// C_CombatDamageTypeMappingData's parse) copies each 0x10 raw row VERBATIM (4 dwords); the
// in-place push sub_182420E30 advances the vector end by 16 -> sizeof PROVEN 0x10. Field order =
// S_CombatAttackHitStatisticsDBData (descriptor-init sub_1800C25F0).
//
// [vs KCD1] combat_attack_hit_statistics does not exist in the KCD1 table-schema extraction
// (table_schemas_v2.json) -- this vertical is new in KCD2.

namespace wh::combatmodule {

class C_CombatAttackHitStatisticsData {
public:
    int32_t combat_attack_type_id;  // +0x00  PK, FK -> combat_attack_type
    int32_t combat_zone_id;         // +0x04  PK, FK -> combat_zone
    int32_t body_subpart_id;        // +0x08  PK, FK -> body_subpart
    int32_t hit_count;              // +0x0C  recorded hit tally
};
static_assert(sizeof(C_CombatAttackHitStatisticsData) == 0x10,
              "C_CombatAttackHitStatisticsData must be 0x10 (push helper sub_182420E30 advances 16)");

}  // namespace wh::combatmodule
