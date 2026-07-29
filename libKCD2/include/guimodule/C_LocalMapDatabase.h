#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTableDatabase.h"
#include "guimodule/S_LocalMap.h"
#include "guimodule/S_LocalMapDBData.h"

// -----------------------------------------------
// wh::guimodule::C_LocalMapDatabase -- the "ui_local_maps" table (KCD2 1.5.6, kd7u).
// sizeof 0x98 (template extent -- global static).
// -----------------------------------------------
// RTTI TD RVA 0x4CAEFA8, COL RVA 0x4139078, vtable 0x183C40838. Stack: plain
// C_ObjectTableDatabase<S_LocalMap, S_LocalMapDBData>.
// GLOBAL STATIC @0x1853268B0 (thunk sub_181946028; table "ui_local_maps", group "ui").
// Register [7] sub_1819494BC (descs qword_1854B1E60, builder sub_1801720A0, 17 cols,
// stride 0x60); ParseRows [10] sub_181507A0C (accessor+converter sub_181507BAC, push
// sub_181507AEC); post-load [16] stub sub_18041A6A0.

namespace wh::guimodule {

class C_LocalMapDatabase
    : public wh::databasemodule::C_ObjectTableDatabase<S_LocalMap, S_LocalMapDBData>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LocalMapDatabase;
};
static_assert(sizeof(C_LocalMapDatabase) == 0x98, "C_LocalMapDatabase must be 0x98 (OTD extent)");

}  // namespace wh::guimodule
