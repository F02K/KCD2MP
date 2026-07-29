#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatActionTypeGroupData -- KCD2 1.5.6 (kd7u). sizeof 0x08.
// -----------------------------------------------
// combat_action_type_group row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_ObjectTableDatabase<C_CombatActionTypeGroupData, S_CombatActionTypeGroupDBData>> =
// C_CombatActionTypeGroupDatabase (vtable 0x183E5A1A0, 22 slots). ParseRows (vtable[10] =
// sub_1814B5BAC, COMDAT-folded with the C_KeyValueData parse) copies each 8-byte raw row
// VERBATIM (2 dwords); the in-place push sub_18240E2D0 advances the vector end by 8 -> sizeof
// PROVEN 0x08. Field order = S_CombatActionTypeGroupDBData (descriptor-init sub_1800C21A0).
//
// [vs KCD1] No KCD1 reference header; the KCD1 table schema (combat_action_type_group,
// rowsize 0x8, same 2 columns) matches -- layout unchanged.

namespace wh::combatmodule {

class C_CombatActionTypeGroupData {
public:
    int32_t combat_action_type_group_id;  // +0x00  PK (group bucket, e.g. 1000..1019)
    int32_t combat_action_type_id;        // +0x04  PK, member action type
};
static_assert(sizeof(C_CombatActionTypeGroupData) == 0x08,
              "C_CombatActionTypeGroupData must be 0x08 (push helper sub_18240E2D0 advances 8)");

}  // namespace wh::combatmodule
