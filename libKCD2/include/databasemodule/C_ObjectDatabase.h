#pragma once
#include <cstdint>
#include "C_ObjectDatabaseBase.h"

// -----------------------------------------------
// wh::databasemodule::C_ObjectDatabase<TObject, TContainer> -- row-owning database layer
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven template (e.g. .?AV?$C_ObjectDatabase@US_ReputationChange@rpgmodule@wh@@Vvector@
// std@@@databasemodule@wh@@ = C_ObjectDatabase<S_ReputationChange, std::vector>).
// Owns the row storage: begin/end @+0x30/+0x38, capacity @+0x40 (zeroed by the inlined
// C_ObjectTableDatabase ctor sub_1811CEFE4; I_DynamicEnum reads the row array through +0x30 with
// the row stride baked into each instantiation -- S_ReputationChange 0x28, S_SoulArchetype 0x58).
// LAYER ATTRIBUTION of +0x30..+0x47 to THIS template (vs the table layer) is [INFERRED] -- the
// ctors are fully inlined into the concrete database ctor, so the split is by template-name
// semantics ("the database whose container holds the objects"), not by separate ctor evidence.
// The engine's real container parameter is the std::vector TEMPLATE; mirrored here as a plain
// member of vector-of-TObject shape.

namespace wh::databasemodule {

template <typename TObject>
class C_ObjectDatabase : public C_ObjectDatabaseBase {
public:
    TObject* m_rowsBegin;      // +0x30  row array (stride = sizeof(TObject))
    TObject* m_rowsEnd;        // +0x38
    TObject* m_rowsCapacity;   // +0x40  std::vector<TObject> {begin,end,cap} shape
};

}  // namespace wh::databasemodule
