#pragma once
#include "C_AnimationDatabaseBase.h"

// -----------------------------------------------
// wh::animationmodule::C_AnimationDatabaseGenerated<TData, TDBData, TPreprocessor> -- KCD2 (kd7u).
// -----------------------------------------------
// RTTI .?AV?$C_AnimationDatabaseGenerated@US_CombatActionAttackData@..@US_CombatActionAttackDataDB
// Data@..@V?$C_CombatActionPreprocessor@...@@animationmodule@wh@@ . CODEGEN wrapper over
// C_AnimationDatabaseBase; supplies generated overrides that run the TPreprocessor over each loaded
// TData row (post-load animation-tag resolution). Adds NO data members of its own -- its ctor
// (attack DB: sub_1810DB470) constructs only the C_AnimationDatabaseBase members (+0x98..+0xE7).
//
// TPreprocessor for combat actions = wh::combatmodule::C_CombatActionPreprocessor<TData>.

namespace wh::animationmodule {

template <typename TData, typename TDBData, typename TPreprocessor>
class C_AnimationDatabaseGenerated : public C_AnimationDatabaseBase<TData, TDBData> {
    // codegen virtual overrides (row preprocessing) only; no new data members
};

}  // namespace wh::animationmodule
