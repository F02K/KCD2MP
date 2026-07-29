#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActionTypeGroupDBData -- KCD2 1.5.6 (kd7u). sizeof 0x08.
// -----------------------------------------------
// Raw combat_action_type_group table row (Libs/Tables/combat/combat_action_type_group.xml):
// membership of action types in group buckets (group ids 1000+ in E_CombatActionType). Row type
// of C_ObjectTableDatabase<C_CombatActionTypeGroupData, S_CombatActionTypeGroupDBData> under
// C_CombatActionTypeGroupDatabase. Register (vtable[7] = sub_181942694) binds the 2-descriptor
// array qword_1854A6B10 with row stride 8 -> sizeof PROVEN 0x08. Both columns are int32
// C_PKColumnDescriptor entries (composite key; static-init sub_1800C21A0).

namespace wh::combatmodule {

struct S_CombatActionTypeGroupDBData {
    int32_t combat_action_type_group_id;  // +0x00  PK (group bucket, e.g. 1000..1019)
    int32_t combat_action_type_id;        // +0x04  PK, member action type
};
static_assert(sizeof(S_CombatActionTypeGroupDBData) == 0x08,
              "S_CombatActionTypeGroupDBData must be 0x08 (registered row stride 8)");

}  // namespace wh::combatmodule
