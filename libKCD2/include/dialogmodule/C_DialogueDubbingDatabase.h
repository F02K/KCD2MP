#pragma once
#include "S_DialogueDubbing.h"
#include "../databasemodule/C_ObjectTreeDatabase.h"
#include "../databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "../databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueDubbingDatabase -- "response_depot_wave_file" tree DB
// (KCD2 1.5.6, kd7u).  sizeof 0x78 (tree-DB extent -- global static singleton).
// -----------------------------------------------
// RTTI TD 0x184B49810; vtable 0x183C12F60 (22 slots); static-init sub_181790898 ->
// sub_1817908E0 -> real ctor sub_181790AD0; deleting-dtor [0] sub_1827F2C30 (-> base
// dtor sub_181AD7188). Stack: C_ObjectDatabaseKeyIndexed<C_ObjectTreeDatabase<
// S_DialogueDubbing>, DefaultKeyExtractor>; adds NO own data -- configures base
// m_namespace@+0x48 = "wh::dialogmodule::", m_name@+0x50 = "response_depot_wave_file",
// m_group@+0x58 = "dialog". Slots: [9] sub_180ED69C8 Load; [15] sub_182027EC0 clear;
// [16] sub_1819ED9A4 sort/RebuildIndices (stride-8 proof); [21] sub_180ED6B6C XML
// deserialize ("database" node; per-element parser sub_180ED6DA8).

namespace wh::dialogmodule {

class C_DialogueDubbingDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<S_DialogueDubbing>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<S_DialogueDubbing>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueDubbingDatabase;
};
static_assert(sizeof(C_DialogueDubbingDatabase) == 0x78, "C_DialogueDubbingDatabase must be 0x78 (tree-DB extent)");

}  // namespace wh::dialogmodule
