#pragma once
#include <cstdint>
#include "E_CombatTag.h"
#include "S_CombatTagTableRow.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatTagData -- KCD2 1.5.6 (kd7u). sizeof 0x18.
// -----------------------------------------------
// combat_tag lookup row -> Data. NON-polymorphic (no own TD). DB stack:
// C_CombatDatabase<C_AnimationDatabaseStaticIndexed<Data, Row>> over
// C_ObjectTableDatabase<Data, Row> (vtable 0x183C02138; StaticIndexed layer vtable 0x183E598F8).
// ParseRows sub_1815E21F4 converts inline: copies the id, wraps combat_tag_name into a
// CryString (sub_1804F66CC), and derives +0x10 by feeding the NAME through the strcmp ladder
// sub_1815E22D8 ("NONE"=0 .. "LAST"=7, see E_CombatTag.h). Non-grow push path advances the
// vector end by 24 (`v1[1] += 24`) -> sizeof VERIFIED 0x18 (grow path sub_18113AECC).
// C_CombatTagData is also the VALUE type of the combat_attack_type_tag / combat_zone_tag
// C_KeyValueData<..., C_CombatTagData> join tables (RTTI).
//
// [vs KCD1] No KCD1 C_CombatTagData header existed; KCD1's combat_tag row had the same columns.

namespace wh::combatmodule {

struct C_CombatTagData {
    int32_t          combat_tag_id;    // +0x00
    int32_t          _pad04;           // +0x04
    CryStringT<char> combat_tag_name;  // +0x08
    E_CombatTag      tag;               // +0x10  parsed from combat_tag_name (sub_1815E22D8)
    int32_t          _pad14;           // +0x14
};
static_assert(sizeof(C_CombatTagData) == 0x18, "C_CombatTagData must be 0x18");

}  // namespace wh::combatmodule
