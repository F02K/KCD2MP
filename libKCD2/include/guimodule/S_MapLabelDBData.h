#pragma once
#include <cstdint>
#include "CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::guimodule::S_MapLabelDBData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// Raw "ui_map_label" table row. Columns from the C_MapLabelDatabase Register descriptor
// builder sub_1801726A0 (5 cols, row stride 0x30 via sub_1819491E0).

namespace wh::guimodule {

struct S_MapLabelDBData {
    CryGUID     ui_map_label_id;   // +0x00  guid, PK
    CryGUID     location_id;       // +0x10  guid
    float       position_x;        // +0x20
    float       position_y;        // +0x24
    const char* ui_name;           // +0x28  string col (type 5)
};
static_assert(sizeof(S_MapLabelDBData) == 0x30, "ui_map_label row stride is 0x30 (sub_1819491E0)");

}  // namespace wh::guimodule
