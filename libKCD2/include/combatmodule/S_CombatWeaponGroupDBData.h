#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatWeaponGroupDBData -- KCD2 1.5.6 (kd7u). sizeof 0x20.
// -----------------------------------------------
// Raw combat_weapon_group table row (Libs/Tables/combat/combat_weapon_group.xml). Row type of
// C_ObjectTableDatabase<C_CombatWeaponGroupData, S_CombatWeaponGroupDBData> (vtable
// 0x183ABDFE0). Row accessor sub_180A20D68 returns `rawBase + 32*i` -> sizeof VERIFIED 0x20.
// Column names/offsets/types from the binder sub_1800C39B0. Row->Data ctor sub_1815B1B80 maps
// +0x00->Data+0x08, +0x08->Data+0x10, +0x10->Data+0x18, +0x18->Data+0x20 (see
// C_CombatWeaponGroupData.h).
//
// [vs KCD1] Same four columns; KCD1's pack(1) row (0x1C) had a 4-byte row header, KCD2 is
// 0-based with natural alignment.

namespace wh::combatmodule {

struct S_CombatWeaponGroupDBData {
    int32_t     combat_weapon_group_id;    // +0x00  PK
    int32_t     _pad04;                    // +0x04
    const char* combat_weapon_group_name;  // +0x08
    const char* mn_tag;                    // +0x10
    int32_t     combat_priority;           // +0x18
    int32_t     _pad1C;                    // +0x1C
};
static_assert(sizeof(S_CombatWeaponGroupDBData) == 0x20,
              "S_CombatWeaponGroupDBData must be 0x20");

}  // namespace wh::combatmodule
