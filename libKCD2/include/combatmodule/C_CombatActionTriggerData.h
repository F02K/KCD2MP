#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatActionTriggerData -- KCD2 1.5.6 (kd7u). sizeof 0x14.
// -----------------------------------------------
// combat_action_trigger row -> Data. NON-polymorphic (no own TD; appears only as a template arg).
// Stack: C_CombatDatabase<C_ObjectTableDatabase<C_CombatActionTriggerData,
// S_CombatActionTriggerDBData>> = C_CombatActionTriggerDatabase (vtable 0x183C39F10, 22 slots).
// ParseRows (vtable[10] = sub_181829D44) copies each 0x14 raw row VERBATIM (5 dwords) into the
// Data vector; the in-place push helper sub_18275B884 advances the vector end by 20 -> sizeof
// PROVEN 0x14. Field order/typing = S_CombatActionTriggerDBData (identity copy, descriptor-init
// sub_1800C1FC0).
//
// [vs KCD1] No KCD1 reference header; the KCD1 table schema (table_schemas_v2.json,
// combat_action_trigger, rowsize 0x14, same 5 columns) matches -- layout unchanged.

namespace wh::combatmodule {

class C_CombatActionTriggerData {
public:
    int32_t combat_action_trigger_id;  // +0x00  PK
    int32_t combat_guard_type_id;      // +0x04  FK -> combat_guard_type
    int32_t combat_input_class_id;     // +0x08  FK -> combat_input_class
    float   timeout;                   // +0x0C
    int32_t combat_action_type_id;     // +0x10  FK -> combat_action_type
};
static_assert(sizeof(C_CombatActionTriggerData) == 0x14,
              "C_CombatActionTriggerData must be 0x14 (push helper sub_18275B884 advances 20)");

}  // namespace wh::combatmodule
