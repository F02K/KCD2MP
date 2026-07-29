#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_KeyValueTableRow<TSource, TDestination> -- KCD2 1.5.6 (kd7u). sizeof 0x08.
// -----------------------------------------------
// Raw row of a generic two-int key->value mapping table. Sole binary instantiation:
// S_KeyValueTableRow<C_CombatActionTypeDataSource, C_CombatActionTypeDataDestination> (RTTI
// U?$S_KeyValueTableRow@...@combatmodule@wh@@), the row type of C_CombatActionTypeMappingDatabase
// (table combat_action_type_mapping). Register (vtable[7] = sub_181854AA0) binds the 2-descriptor
// array qword_184929E10 with row stride 8 -> sizeof PROVEN 0x08. Both columns are int32 PK
// descriptors (composite key; static-init sub_1800AE910: "src_action_type_id" @+0,
// "dst_action_type_id" @+4). Column names/id domains come from the TSource/TDestination tag
// classes; the members themselves are plain int32 ids in the only instantiation, so they are
// modeled as int32_t [member NAMES inferred from the template's key/value role].

namespace wh::combatmodule {

template <typename TSource, typename TDestination>
struct S_KeyValueTableRow {
    int32_t m_key;    // +0x00  PK, source-side id  (src_action_type_id in the sole instantiation)
    int32_t m_value;  // +0x04  PK, destination-side id (dst_action_type_id)
};

}  // namespace wh::combatmodule
