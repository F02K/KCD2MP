#pragma once

// -----------------------------------------------
// wh::combatmodule::C_CombatActionTypeDataSource -- KCD2 1.5.6 (kd7u). Tag/traits class.
// -----------------------------------------------
// Compile-time SOURCE-side parameter of the key-value vertical
// C_KeyValueDatabase<C_CombatActionTypeDataSource, C_CombatActionTypeDataDestination> /
// C_KeyValueData<...> / S_KeyValueTableRow<...> (= C_CombatActionTypeMappingDatabase, table
// combat_action_type_mapping). Selects the "src_action_type_id" key column (descriptor
// static-init sub_1800AE910 builds src/dst PK descriptors for that instantiation).
// [INFERRED shape] The class appears ONLY inside template-argument manglings -- it is never
// constructed and has no TD/COL/vtable of its own, so it is modeled as an empty tag type; any
// static column-name providers it may carry are fully inlined and unrecoverable.

namespace wh::combatmodule {

class C_CombatActionTypeDataSource {};

}  // namespace wh::combatmodule
