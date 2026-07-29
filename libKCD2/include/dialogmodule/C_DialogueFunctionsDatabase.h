#pragma once
#include "S_DialogueFunction.h"
#include "S_DialogueFunctionsDBData.h"
#include "../databasemodule/C_ObjectTableDatabase.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueFunctionsDatabase -- the "dialogue_functions" table
// (KCD2 1.5.6, kd7u).  sizeof 0x98 (OTD extent -- global static singleton).
// -----------------------------------------------
// RTTI TD 0x184B49550; vtable 0x183C409C0 (22 slots); registrar/ctor sub_181946144;
// deleting-dtor [0] sub_1827F2C98. Stack: plain C_ObjectTableDatabase<S_DialogueFunction,
// S_DialogueFunctionsDBData>; adds NO own data -- configures base m_tableName@+0x50 =
// "dialogue_functions", m_groupName@+0x70 = "text". Slots: [7] sub_18194432C Register;
// [9] sub_180EF3A4C base Load; [10] sub_1815B0714 ParseRows (row iter sub_1813E4338,
// emplace sub_1815B0854); [14] sub_18156967C; [21] sub_181A7E520 base table extra.
// Hot-reloaded by console command wh_dlg_reloadFunctions.

namespace wh::dialogmodule {

class C_DialogueFunctionsDatabase
    : public wh::databasemodule::C_ObjectTableDatabase<S_DialogueFunction, S_DialogueFunctionsDBData>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueFunctionsDatabase;
};
static_assert(sizeof(C_DialogueFunctionsDatabase) == 0x98, "C_DialogueFunctionsDatabase must be 0x98 (OTD extent)");

}  // namespace wh::dialogmodule
