#pragma once
#include "../animationmodule/C_ActionDatabaseGenerated.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatDatabaseGenerated<TData, TDBData, TQuery> -- KCD2 1.5.6 (kd7u).
// -----------------------------------------------
// RTTI .?AV?$C_CombatDatabaseGenerated@US_CombatActionAttackData@..@US_CombatActionAttackDataDBData@
// ..@US_CombatActionAttackQueryData@..@@combatmodule@wh@@ . Outermost CODEGEN wrapper of the combat
// action databases (one instantiation per combat table). Binds TPreprocessor to
// C_CombatActionPreprocessor<TData> and drives table registration:
//   ctor (attack DB: sub_1810DAD84) assigns the inherited table/group name strings
//   (C_ObjectTableDatabase::m_tableName @+0x50 = "combat_action_attack", m_groupName @+0x70 =
//   "combat") and pushes a loader/registration record into the query-selector list (+0xF0). It adds
//   NO data members of its own.
//
// Note: KCD2's action-type databases (C_CombatActionTypeDatabase etc.) additionally register by NAME
// in a global std::map keyed by CryString (see combatmodule_port/enums_data.md); that name->onLoad
// registration is separate from this row-database chain.

namespace wh::combatmodule {

struct S_CombatActionAttackData;
struct S_CombatActionAttackDataDBData;
struct S_CombatActionAttackQueryData;
template <typename TData> class C_CombatActionPreprocessor;   // combatmodule (animation post-load)

template <typename TData, typename TDBData, typename TQuery>
class C_CombatDatabaseGenerated
    : public wh::animationmodule::C_ActionDatabaseGenerated<
          TData, TDBData, TQuery, C_CombatActionPreprocessor<TData>> {
    // codegen table-registration overrides only; no new data members
};

}  // namespace wh::combatmodule
