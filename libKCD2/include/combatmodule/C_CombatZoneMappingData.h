#pragma once
#include <cstdint>
#include "S_CombatZoneMappingTableRow.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatZoneMappingData -- KCD2 1.5.6 (kd7u). sizeof 0x18.
// -----------------------------------------------
// combat_zone_mapping row -> Data. NON-polymorphic POD, layout IDENTICAL to
// S_CombatZoneMappingTableRow (verbatim 6-dword copy, no FK resolution). DB stack:
// C_CombatDatabase<C_AnimationDatabaseStatic<Data, Row>> over
// C_ObjectTableDatabase<Data, Row> (vtable 0x183C3F138). ParseRows sub_181720A3C copies row
// dwords [0..5] into a stack temp and pushes it; push helper sub_182420E54 advances the
// vector end by 24 -> sizeof VERIFIED 0x18 (grow path sub_1806F8140).
//
// [vs KCD1] KCD1's C_CombatZoneMappingData (also 0x18 POD) was documented with usage-derived
// member names (m_matchKey/m_outputZone/m_zoneId/...); KCD2 members use the AUTHORITATIVE
// column names from the binder. Column order src,dst,attack_type,weapon_class,input_class,
// action_type is the binder/row order, NOT the alphabetized XML order.

namespace wh::combatmodule {

struct C_CombatZoneMappingData {
    int32_t src_zone_id;            // +0x00
    int32_t dst_zone_id;            // +0x04
    int32_t combat_attack_type_id;  // +0x08
    int32_t weapon_class_id;        // +0x0C
    int32_t combat_input_class_id;  // +0x10
    int32_t combat_action_type_id;  // +0x14
};
static_assert(sizeof(C_CombatZoneMappingData) == 0x18,
              "C_CombatZoneMappingData must be 0x18");

}  // namespace wh::combatmodule
