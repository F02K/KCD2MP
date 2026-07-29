#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActionTypeTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// Raw combat_action_type table row (Libs/Tables/combat/combat_action_type.xml). Row type of
// C_AnimationDatabaseStaticIndexed<C_CombatActionTypeData, S_CombatActionTypeTableRow> under
// C_CombatActionTypeDatabase. Register (vtable[7] = sub_181942788) binds the 2-descriptor array
// qword_1854A6B00 with row stride 16 -> sizeof PROVEN 0x10. Columns from descriptor static-init
// sub_1800C2270: id (PK, int32 @+0) and combat_action_type_name (type 5 = string, 8 bytes @+8).
// The string cell holds a load-time-patched char pointer: ParseRows (sub_1813E3CA4) passes
// *(char**)(row+8) straight to the CryStringT-from-cstr ctor sub_1804F66CC.

namespace wh::combatmodule {

struct S_CombatActionTypeTableRow {
    int32_t     combat_action_type_id;    // +0x00  PK (id domain of E_CombatActionType)
    int32_t     _pad04;                   // +0x04
    const char* combat_action_type_name;  // +0x08  patched string pointer
};
static_assert(sizeof(S_CombatActionTypeTableRow) == 0x10,
              "S_CombatActionTypeTableRow must be 0x10 (registered row stride 16)");

}  // namespace wh::combatmodule
