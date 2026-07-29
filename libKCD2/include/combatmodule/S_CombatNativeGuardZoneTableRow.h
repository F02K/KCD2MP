#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatNativeGuardZoneTableRow -- KCD2 1.5.6 (kd7u). sizeof 0x18.
// -----------------------------------------------
// Raw combat_native_guard_zone table row (Libs/Tables/combat/combat_native_guard_zone.xml).
// Row type of C_ObjectTableDatabase<C_CombatNativeGuardZoneData, S_CombatNativeGuardZoneTableRow>
// (vtable 0x183C3F5A8); its ParseRows override sub_1816F0454 walks the raw buffer with stride
// 0x18 (`rawBase + 24*i`) -> sizeof VERIFIED 0x18. Column names/offsets/types from the column
// binder sub_1800C3210 (C_ColumnDescriptor: type @+0x08, row offset @+0x0C, name @+0x18).
// The first four columns are C_PKColumnDescriptor (composite primary key).
//
// [vs KCD1] Same six columns, but KCD1 rows carried a 4-byte row-header dword (columns started
// at +0x04, total 0x18 via pack(1)); KCD2 columns are 0-based.

namespace wh::combatmodule {

struct S_CombatNativeGuardZoneTableRow {
    int32_t combat_guard_stance_id;      // +0x00  PK
    int32_t combat_zone_id;              // +0x04  PK
    int32_t r_weapon_class_id;           // +0x08  PK
    int32_t l_weapon_class_id;           // +0x0C  PK
    int32_t r_combat_weapon_group_id;    // +0x10  default -1 (descriptor default-value flag 0x101)
    int32_t l_combat_weapon_group_id;    // +0x14  default -1
};
static_assert(sizeof(S_CombatNativeGuardZoneTableRow) == 0x18,
              "S_CombatNativeGuardZoneTableRow must be 0x18");

}  // namespace wh::combatmodule
