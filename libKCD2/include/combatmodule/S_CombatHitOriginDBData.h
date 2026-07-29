#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatHitOriginDBData -- KCD2 1.5.6 (kd7u). sizeof 0x20.
// -----------------------------------------------
// Raw combat_hit_origin table row (Libs/Tables/combat/combat_hit_origin.xml): incoming-hit
// direction sectors (forward/backward/left/right) as [angle_from, angle_to] arcs. Row type of
// C_AnimationDatabaseStaticIndexed<C_CombatHitOriginData, S_CombatHitOriginDBData> under
// C_CombatHitOriginDatabase. Register (vtable[7] = sub_181941FE8) binds the 5-descriptor array
// qword_1854A6C18 with row stride 32 -> sizeof PROVEN 0x20. Columns from descriptor static-init
// sub_1800C2DF0: id (PK), name (string), mn_tag (string), angle_from / angle_to (type 2 = float).

namespace wh::combatmodule {

struct S_CombatHitOriginDBData {
    int32_t     combat_hit_origin_id;    // +0x00  PK
    int32_t     _pad04;                  // +0x04
    const char* combat_hit_origin_name;  // +0x08  patched string pointer
    const char* mn_tag;                  // +0x10  Mannequin tag (e.g. "ho_forward")
    float       angle_from;              // +0x18  arc start, degrees
    float       angle_to;                // +0x1C  arc end, degrees
};
static_assert(sizeof(S_CombatHitOriginDBData) == 0x20,
              "S_CombatHitOriginDBData must be 0x20 (registered row stride 32)");

}  // namespace wh::combatmodule
