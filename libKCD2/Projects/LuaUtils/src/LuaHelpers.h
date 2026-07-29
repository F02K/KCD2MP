// Lua <-> engine value conversion shared by both scriptbinds.
//
// Ids (item / entity / inventory WUIDs) cross Lua as ScriptHandle
// (ANY_THANDLE, 64-bit nHandle payload) - the same representation the vanilla
// binds use (GetInventoryTable elements, GetItem.id). Lua numbers are 32-bit-
// precision floats in this VM and would corrupt a 64-bit WUID.

#pragma once

#include <cstdint>

#include "crysystem/ScriptAnyValue.h"

namespace Offsets {
struct IFunctionHandler;
struct IScriptSystem;
struct IScriptTable;
}
namespace wh::entitymodule { class C_Item; }

namespace luautils {

// ANY_THANDLE wrapper for a 64-bit id.
inline ScriptAnyValue HandleValue(uint64_t value)
{
    ScriptAnyValue any;
    any.type = ANY_THANDLE;
    any.nHandle = (int64_t)value;
    return any;
}

// Typed handle read of 1-based param nIdx (raises a script error on mismatch,
// same as every other typed GetParam).
bool GetHandleParam(Offsets::IFunctionHandler* pH, int nIdx, uint64_t& out);

// The per-item table returned by GetItemEx/GetInventoryEx:
//   { id, className, type, amount, health, condition, quality, maxQuality,
//     isEquipped, owner, entity }
// Caller pushes it to Lua (EndFunctionAny/SetAtAny) and then Release()s the
// wrapper - the pushed Lua value keeps the table alive on the script side.
Offsets::IScriptTable* BuildItemTable(Offsets::IScriptSystem* pSS, wh::entitymodule::C_Item* item);

}  // namespace luautils
