#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatTagTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// Raw combat_tag table row (Libs/Tables/combat/combat_tag.xml). Row type of
// C_ObjectTableDatabase<C_CombatTagData, S_CombatTagTableRow> (vtable 0x183C02138).
// ParseRows sub_1815E21F4 walks the raw buffer with stride 0x10 (`rawBase + 16*i`)
// -> sizeof VERIFIED 0x10. Columns from the binder sub_1800C3690: combat_tag_id PK @+0x00,
// combat_tag_name (string, type 5) @+0x08.
//
// [vs KCD1] Same two columns; KCD1's row put them at +0x04/+0x08 behind a 4-byte row header.

namespace wh::combatmodule {

struct S_CombatTagTableRow {
    int32_t     combat_tag_id;    // +0x00  PK
    int32_t     _pad04;           // +0x04
    const char* combat_tag_name;  // +0x08
};
static_assert(sizeof(S_CombatTagTableRow) == 0x10, "S_CombatTagTableRow must be 0x10");

}  // namespace wh::combatmodule
