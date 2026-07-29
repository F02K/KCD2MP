#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatZoneDistanceDBData -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// Raw combat_zone_distance table row (Libs/Tables/combat/combat_zone_distance.xml) -- distance
// metrics of a src->dst zone transition. Row type of
// C_ObjectTableDatabase<C_CombatZoneDistanceData, S_CombatZoneDistanceDBData> (vtable
// 0x183C380D0). ParseRows sub_18164FCB0 walks the raw buffer with stride 0x10
// (`rawBase + 16*i`) -> sizeof VERIFIED 0x10. Columns from the binder sub_1800C7A60;
// BOTH distances are descriptor type 2 = FLOAT (combat_distance holds 0/1/2 values in the
// XML but is stored as float).
//
// [vs KCD1] KCD1's row (0x18) also had visual_guard_distance/defense_guard_distance; KCD2
// DROPPED both (the column-name strings do not exist in the KCD2 binary -- the XML still
// carries them, unbound).

namespace wh::combatmodule {

struct S_CombatZoneDistanceDBData {
    int32_t src_combat_zone_id;  // +0x00  PK
    int32_t dst_combat_zone_id;  // +0x04  PK
    float   rpg_distance;        // +0x08
    float   combat_distance;     // +0x0C
};
static_assert(sizeof(S_CombatZoneDistanceDBData) == 0x10,
              "S_CombatZoneDistanceDBData must be 0x10");

}  // namespace wh::combatmodule
