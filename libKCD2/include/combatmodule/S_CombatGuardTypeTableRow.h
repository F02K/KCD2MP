#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatGuardTypeTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x28.
// -----------------------------------------------
// Raw combat_guard_type table row (Libs/Tables/combat/combat_guard_type.xml). Row type of
// C_AnimationDatabaseStaticIndexed<C_CombatGuardTypeData, S_CombatGuardTypeTableRow> under
// C_CombatGuardTypeDatabase. Register (vtable[7] = sub_1819420DC) binds the 9-descriptor array
// qword_1854A6BD0 with row stride 40 -> sizeof PROVEN 0x28. Columns from descriptor static-init
// sub_1800C2B40: id (PK), name (string), mn_tag / end_mn_tag (strings, default "" via
// sub_180D16794), move_restriction (type 4 = bool, no default), then four bools with defaults
// via the bool-with-default builder sub_18197AAE8: sync_guard(false), has_auto_transition(TRUE),
// is_asymmetric(false), hidden_combat_ui(false).

namespace wh::combatmodule {

struct S_CombatGuardTypeTableRow {
    int32_t     combat_guard_type_id;    // +0x00  PK
    int32_t     _pad04;                  // +0x04
    const char* combat_guard_type_name;  // +0x08  patched string pointer
    const char* mn_tag;                  // +0x10  guard-enter Mannequin tag (default "")
    const char* end_mn_tag;              // +0x18  guard-exit Mannequin tag (default "")
    bool        move_restriction;        // +0x20
    bool        sync_guard;              // +0x21  default false
    bool        has_auto_transition;     // +0x22  default TRUE
    bool        is_asymmetric;           // +0x23  default false
    bool        hidden_combat_ui;        // +0x24  default false
    uint8_t     _pad25[3];               // +0x25
};
static_assert(sizeof(S_CombatGuardTypeTableRow) == 0x28,
              "S_CombatGuardTypeTableRow must be 0x28 (registered row stride 40)");

}  // namespace wh::combatmodule
