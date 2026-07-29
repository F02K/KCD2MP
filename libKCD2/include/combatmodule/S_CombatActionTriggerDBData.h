#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatActionTriggerDBData -- KCD2 1.5.6 (kd7u). sizeof 0x14.
// -----------------------------------------------
// Raw combat_action_trigger table row (Libs/Tables/combat/combat_action_trigger.xml). Row type of
// C_ObjectTableDatabase<C_CombatActionTriggerData, S_CombatActionTriggerDBData> (RTTI
// .?AV?$C_ObjectTableDatabase@VC_CombatActionTriggerData@combatmodule@wh@@US_CombatActionTriggerDBData@23@@databasemodule@wh@@).
// Register (C_CombatActionTriggerDatabase vtable[7] = sub_18194287C) binds the 5-descriptor array
// qword_1854A6AD8 and requests the table with row stride 20 -> sizeof PROVEN 0x14.
// Column names/types/offsets verbatim from the descriptor static-init sub_1800C1FC0
// (C_PKColumnDescriptor/C_ColumnDescriptor writes; type codes: 0=int32, 2=float).

namespace wh::combatmodule {

struct S_CombatActionTriggerDBData {
    int32_t combat_action_trigger_id;  // +0x00  PK
    int32_t combat_guard_type_id;      // +0x04  FK -> combat_guard_type
    int32_t combat_input_class_id;     // +0x08  FK -> combat_input_class
    float   timeout;                   // +0x0C  descriptor type 2 = float
    int32_t combat_action_type_id;     // +0x10  FK -> combat_action_type
};
static_assert(sizeof(S_CombatActionTriggerDBData) == 0x14,
              "S_CombatActionTriggerDBData must be 0x14 (registered row stride 20)");

}  // namespace wh::combatmodule
