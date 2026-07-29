#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatSideDBData -- KCD2 1.5.6 (kd7u). sizeof 0x28.
// -----------------------------------------------
// Raw combat_side table row (Libs/Tables/combat/combat_side.xml). Row type of
// C_ObjectTableDatabase<C_CombatSideData, S_CombatSideDBData> (vtable 0x183ABDE70).
// Row accessor sub_181430540 returns `rawBase + 40*i` -> sizeof VERIFIED 0x28.
// Column names/offsets/types from the column binder sub_1800C3450 (angle_from/angle_to are
// descriptor type 2 = FLOAT; string columns type 5 are raw char pointers patched to absolute
// at table load, not CryStringT objects).
//
// [vs KCD1] Same six columns; KCD1's auto-generated row (0x24, pack(1)) had a 4-byte row-header
// dword before the PK. KCD2 columns are 0-based with natural alignment (pad after the PK).

namespace wh::combatmodule {

struct S_CombatSideDBData {
    int32_t     combat_side_id;         // +0x00  PK
    int32_t     _pad04;                 // +0x04
    const char* combat_side_name;       // +0x08
    const char* mn_tag;                 // +0x10
    float       angle_from;             // +0x18
    float       angle_to;               // +0x1C
    int32_t     combat_action_type_id;  // +0x20
    int32_t     _pad24;                 // +0x24
};
static_assert(sizeof(S_CombatSideDBData) == 0x28, "S_CombatSideDBData must be 0x28");

}  // namespace wh::combatmodule
