#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTableDatabase.h"
#include "guimodule/S_MapLabel.h"
#include "guimodule/S_MapLabelDBData.h"

// -----------------------------------------------
// wh::guimodule::C_MapLabelDatabase -- the "ui_map_label" table (KCD2 1.5.6, kd7u).
// sizeof 0x98 (template extent -- global static).
// -----------------------------------------------
// RTTI TD RVA 0x4CAF0B8, COL RVA 0x41390A0, vtable 0x183C406B8. Stack: plain
// C_ObjectTableDatabase<S_MapLabel, S_MapLabelDBData>.
// GLOBAL STATIC @0x185326950 (thunk sub_181945F0C; table "ui_map_label", group "ui").
// Register [7] sub_1819491E0 (descs qword_1854B1FA0, builder sub_1801726A0, 5 cols,
// stride 0x30); ParseRows [10] sub_180D0FFD4 (push sub_182B24A1C, grow sub_180D0FE4C);
// post-load [16] stub sub_18041A6A0.

namespace wh::guimodule {

class C_MapLabelDatabase
    : public wh::databasemodule::C_ObjectTableDatabase<S_MapLabel, S_MapLabelDBData>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MapLabelDatabase;
};
static_assert(sizeof(C_MapLabelDatabase) == 0x98, "C_MapLabelDatabase must be 0x98 (OTD extent)");

}  // namespace wh::guimodule
