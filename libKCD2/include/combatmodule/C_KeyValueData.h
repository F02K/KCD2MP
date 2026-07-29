#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_KeyValueData<TSource, TDestination> -- KCD2 1.5.6 (kd7u). sizeof 0x08.
// -----------------------------------------------
// Data element of a generic two-int key->value mapping table. Sole binary instantiation:
// C_KeyValueData<C_CombatActionTypeDataSource, C_CombatActionTypeDataDestination> (RTTI
// V?$C_KeyValueData@...@combatmodule@wh@@) inside the stack C_CombatDatabase<
// C_AnimationDatabaseStatic<C_KeyValueData<...>, S_KeyValueTableRow<...>>> =
// C_CombatActionTypeMappingDatabase (final vtable 0x183C436A0; mid-ctor vtable progression in
// sub_181979660 / sub_180EF55C4 shows C_KeyValueDatabase<...> and the C_CombatDatabase layer).
// ParseRows (vtable[10] = sub_1814B5BAC, COMDAT-folded with C_CombatActionTypeGroupData's parse)
// copies each 8-byte raw row VERBATIM (2 dwords); the in-place push sub_18240E2D0 advances the
// vector end by 8 -> sizeof PROVEN 0x08. Member NAMES are inferred from the template's key/value
// role (the raw copy carries no names).
//
// [vs KCD1] combat_action_type_mapping does not exist in the KCD1 table-schema extraction
// (table_schemas_v2.json) -- this vertical is new in KCD2.

namespace wh::combatmodule {

template <typename TSource, typename TDestination>
class C_KeyValueData {
public:
    int32_t m_key;    // +0x00  source-side id  (src_action_type_id in the sole instantiation)
    int32_t m_value;  // +0x04  destination-side id (dst_action_type_id)
};

}  // namespace wh::combatmodule
