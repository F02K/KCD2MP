#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatHitTypeDBData -- KCD2 1.5.6 (kd7u). sizeof 0x18.
// -----------------------------------------------
// Raw combat_hit_type table row (Libs/Tables/combat/combat_hit_type.xml): hit severity classes
// (minor/major). Row type of C_AnimationDatabaseStaticIndexed<C_CombatHitTypeData,
// S_CombatHitTypeDBData> under C_CombatHitTypeDatabase. Register (vtable[7] = sub_18194763C)
// binds the 3-descriptor array qword_1854A6C40 with row stride 24 -> sizeof PROVEN 0x18.
// Columns from descriptor static-init sub_1800C2FE0: id (PK), name (string), mn_tag (string).

namespace wh::combatmodule {

struct S_CombatHitTypeDBData {
    int32_t     combat_hit_type_id;    // +0x00  PK
    int32_t     _pad04;                // +0x04
    const char* combat_hit_type_name;  // +0x08  patched string pointer ("minor"/"major")
    const char* mn_tag;                // +0x10  Mannequin tag (e.g. "hitTypeLow")
};
static_assert(sizeof(S_CombatHitTypeDBData) == 0x18,
              "S_CombatHitTypeDBData must be 0x18 (registered row stride 24)");

}  // namespace wh::combatmodule
