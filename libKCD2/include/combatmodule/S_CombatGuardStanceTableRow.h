#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatGuardStanceTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x20.
// -----------------------------------------------
// Raw combat_guard_stance table row (Libs/Tables/combat/combat_guard_stance.xml). Row type of
// C_AnimationDatabaseStaticIndexed<C_CombatGuardStanceData, S_CombatGuardStanceTableRow> under
// C_CombatGuardStanceDatabase. Register (vtable[7] = sub_1819421D0) binds the 4-descriptor array
// qword_1854A6BB0 with row stride 32 -> sizeof PROVEN 0x20. Columns from descriptor static-init
// sub_1800C29E0: id (PK), name (string), mn_tag / opp_mn_tag (strings with default "" via the
// string-with-default builder sub_180D16794). String cells hold load-time-patched char pointers
// (ParseRows sub_1816F12A4 feeds them to the CryStringT-from-cstr ctor sub_1804F66CC).

namespace wh::combatmodule {

struct S_CombatGuardStanceTableRow {
    int32_t     combat_guard_stance_id;    // +0x00  PK
    int32_t     _pad04;                    // +0x04
    const char* combat_guard_stance_name;  // +0x08  patched string pointer
    const char* mn_tag;                    // +0x10  Mannequin tag (default "")
    const char* opp_mn_tag;                // +0x18  opponent-side Mannequin tag (default "")
};
static_assert(sizeof(S_CombatGuardStanceTableRow) == 0x20,
              "S_CombatGuardStanceTableRow must be 0x20 (registered row stride 32)");

}  // namespace wh::combatmodule
