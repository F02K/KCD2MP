#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatZoneMappingTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x18.
// -----------------------------------------------
// Raw combat_zone_mapping table row (Libs/Tables/combat/combat_zone_mapping.xml) -- maps a
// (src zone, dst zone, attack type, weapon class, input class) tuple to a combat action type.
// Row type of C_ObjectTableDatabase<C_CombatZoneMappingData, S_CombatZoneMappingTableRow>
// (vtable 0x183C3F138). ParseRows sub_181720A3C walks the raw buffer with stride 0x18
// (`rawBase + 24*i`) -> sizeof VERIFIED 0x18. Column names/offsets from the binder
// sub_1800C07B0; the first FIVE columns are the composite PK, combat_action_type_id is the
// payload.
//
// [vs KCD1] Same column set; KCD1's 4-byte row header is gone (0-based offsets).

namespace wh::combatmodule {

struct S_CombatZoneMappingTableRow {
    int32_t src_zone_id;            // +0x00  PK
    int32_t dst_zone_id;            // +0x04  PK
    int32_t combat_attack_type_id;  // +0x08  PK
    int32_t weapon_class_id;        // +0x0C  PK
    int32_t combat_input_class_id;  // +0x10  PK
    int32_t combat_action_type_id;  // +0x14
};
static_assert(sizeof(S_CombatZoneMappingTableRow) == 0x18,
              "S_CombatZoneMappingTableRow must be 0x18");

}  // namespace wh::combatmodule
