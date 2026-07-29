#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatInputClassTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x18.
// -----------------------------------------------
// Raw combat_input_class table row (Libs/Tables/combat/combat_input_class.xml): the player-input
// verb classes (attack_light/heavy/special, move_*, block; id domain of E_CombatInputClass).
// Row type of C_AnimationDatabaseStaticIndexed<C_CombatInputClassData, S_CombatInputClassTableRow>
// under C_CombatInputClassDatabase. Register (vtable[7] = sub_181941EF4) binds the 3-descriptor
// array qword_1854A6C58 with row stride 24 -> sizeof PROVEN 0x18. Columns from descriptor
// static-init sub_1800C3110: id (PK), name (string), mn_tag (string, default "" via the
// specialized builder sub_1819C5C2C).

namespace wh::combatmodule {

struct S_CombatInputClassTableRow {
    int32_t     combat_input_class_id;    // +0x00  PK (id domain of E_CombatInputClass)
    int32_t     _pad04;                   // +0x04
    const char* combat_input_class_name;  // +0x08  patched string pointer
    const char* mn_tag;                   // +0x10  Mannequin tag (default "")
};
static_assert(sizeof(S_CombatInputClassTableRow) == 0x18,
              "S_CombatInputClassTableRow must be 0x18 (registered row stride 24)");

}  // namespace wh::combatmodule
