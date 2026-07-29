#pragma once
#include <cstdint>
#include "S_CombatZoneDistanceDBData.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatZoneDistanceData -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// combat_zone_distance row -> Data. NON-polymorphic POD, layout IDENTICAL to
// S_CombatZoneDistanceDBData (verbatim 4-dword copy, no FK resolution). DB stack:
// C_CombatDatabase<C_ObjectDatabaseKeyIndexed<C_ObjectTableDatabase<Data, Row>,
// C_ObjectDatabaseDefaultKeyExtractor<Data>>> (OTD vtable 0x183C380D0).
// ParseRows sub_18164FCB0 copies row dwords [0..3] into a stack temp and pushes it;
// push helper sub_182420E30 advances the vector end by 16 -> sizeof VERIFIED 0x10
// (grow path sub_180872028).
//
// [vs KCD1] visual_guard_distance/defense_guard_distance dropped (see the row header).

namespace wh::combatmodule {

struct C_CombatZoneDistanceData {
    int32_t src_combat_zone_id;  // +0x00
    int32_t dst_combat_zone_id;  // +0x04
    float   rpg_distance;        // +0x08
    float   combat_distance;     // +0x0C
};
static_assert(sizeof(C_CombatZoneDistanceData) == 0x10,
              "C_CombatZoneDistanceData must be 0x10");

}  // namespace wh::combatmodule
