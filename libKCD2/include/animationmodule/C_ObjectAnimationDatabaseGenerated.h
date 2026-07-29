#pragma once
#include "../databasemodule/C_ObjectDatabaseGenerated.h"
#include "../databasemodule/C_ObjectTableDatabase.h"

// -----------------------------------------------
// wh::animationmodule::C_ObjectAnimationDatabaseGenerated<TData, TDBData> -- KCD2 1.5.6 (kd7u).
// -----------------------------------------------
// RTTI .?AV?$C_ObjectAnimationDatabaseGenerated@US_CombatActionAttackData@..@US_CombatActionAttack
// DataDBData@..@@animationmodule@wh@@ . CODEGEN wrapper: derives from
// C_ObjectDatabaseGenerated<C_ObjectTableDatabase<TData,TDBData>> and adds NO data of its own.
// (RTTI base-count analysis puts the I_AdbChangeListener MI subobject entering just above this
// layer; for modeling the AdbChangeListener base + the animation object index are carried by
// C_AnimationDatabaseBase, which is layout-equivalent since every layer between here and there adds
// no other members.)

namespace wh::animationmodule {

template <typename TData, typename TDBData>
class C_ObjectAnimationDatabaseGenerated
    : public wh::databasemodule::C_ObjectDatabaseGenerated<
          wh::databasemodule::C_ObjectTableDatabase<TData, TDBData>> {
    // codegen virtual overrides only; no new data members
};

}  // namespace wh::animationmodule
