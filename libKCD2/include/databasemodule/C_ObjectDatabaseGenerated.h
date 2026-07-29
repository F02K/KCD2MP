#pragma once
#include "C_ObjectTableDatabase.h"

// -----------------------------------------------
// wh::databasemodule::C_ObjectDatabaseGenerated<TTableDatabase> -- KCD2 1.5.6 (kd7u).
// -----------------------------------------------
// RTTI .?AV?$C_ObjectDatabaseGenerated@V?$C_ObjectTableDatabase@...@databasemodule@wh@@ .
// A CODEGEN wrapper layer (Warhorse's table-codegen emits one per concrete database). It derives
// from the table-database template passed as its single argument and adds NO data members of its
// own -- it only supplies generated virtual overrides (row parse/convert glue). Proven: the attack
// database ctor chain (sub_1810DB52C, the C_ObjectTableDatabase ctor) writes members only up to
// +0x94 (m_isLoaded); the next distinct member (+0x98) is the I_AdbChangeListener MI subobject
// introduced further up the chain. So this layer contributes 0 bytes.
//
// Modeled with TTableDatabase = C_ObjectTableDatabase<TRow, TDBData> for the combat-action DBs.

namespace wh::databasemodule {

template <typename TTableDatabase>
class C_ObjectDatabaseGenerated : public TTableDatabase {
    // codegen virtual overrides only; no new data members
};

}  // namespace wh::databasemodule
