#pragma once
#include "S_LookAtOffset.h"
#include "../databasemodule/C_ObjectTreeDatabase.h"
#include "../databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "../databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"

// -----------------------------------------------
// wh::dialogmodule::C_LookAtOffsetDatabase -- "look_at_offsets" tree DB
// (KCD2 1.5.6, kd7u).  sizeof 0x78 (tree-DB extent -- global static singleton).
// -----------------------------------------------
// RTTI TD 0x184B49368; vtable 0x183C12EA8 (22 slots); static-init sub_1817908BC ->
// sub_181790980 -> real ctor sub_181790A14; deleting-dtor [0] sub_1827F2C64. Stack:
// C_ObjectDatabaseKeyIndexed<C_ObjectTreeDatabase<S_LookAtOffset>, DefaultKeyExtractor>;
// adds NO own data -- configures base m_namespace@+0x48 = "wh::dialogmodule::",
// m_name@+0x50 = "look_at_offsets", m_group@+0x58 = "skald". Slots: [9] sub_181862A60
// Load; [15] sub_1827F3A54 clear (-> sub_181862BFC, stride-24 proof); [16]
// sub_1819ED944 sort/RebuildIndices; [21] sub_181862C30 XML deserialize (per-element
// parser sub_181379D84).

namespace wh::dialogmodule {

class C_LookAtOffsetDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<S_LookAtOffset>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<S_LookAtOffset>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LookAtOffsetDatabase;
};
static_assert(sizeof(C_LookAtOffsetDatabase) == 0x78, "C_LookAtOffsetDatabase must be 0x78 (tree-DB extent)");

}  // namespace wh::dialogmodule
