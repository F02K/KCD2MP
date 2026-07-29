#pragma once
#include <cstdint>
#include "CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::guimodule::S_LocalMapDBData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (PACKED).
// -----------------------------------------------
// Raw "ui_local_maps" table row. Columns from the C_LocalMapDatabase Register
// descriptor builder sub_1801720A0 (17 cols, row stride 0x60 via sub_1819494BC).
// The location_id CryGUID sits at +0x44 (descriptor-proven) -- NOT natural 8-alignment,
// so the row is modelled under pack(4) to reproduce the raw table buffer layout.

namespace wh::guimodule {

#pragma pack(push, 4)
struct S_LocalMapDBData {
    CryGUID     ui_local_map_id;            // +0x00  guid, PK (desc @0x1801720CD, type 3)
    const char* ui_local_map_name;          // +0x10  string col (desc @0x180172121, type 5)
    float       position_x;                 // +0x18
    float       position_y;                 // +0x1C
    float       width;                      // +0x20
    float       height;                     // +0x24
    float       active_x;                   // +0x28
    float       active_y;                   // +0x2C
    float       active_width;               // +0x30
    float       active_height;              // +0x34
    bool        active_area_ellipse;        // +0x38  type 4, size 1
    uint8_t     _pad39[3];                  // +0x39
    int32_t     tile_width;                 // +0x3C
    int32_t     tile_height;                // +0x40
    CryGUID     location_id;                // +0x44  type 3 (unaligned -- see header note)
    bool        enabled_by_default;         // +0x54
    bool        enabled_only_when_inside;   // +0x55
    uint8_t     _pad56[2];                  // +0x56
    int32_t     priority;                   // +0x58  default 0 (has-default 257; helper sub_181AB8D70)
    uint8_t     _pad5C[4];                  // +0x5C
};
#pragma pack(pop)
static_assert(sizeof(S_LocalMapDBData) == 0x60, "ui_local_maps row stride is 0x60 (sub_1819494BC)");
static_assert(offsetof(S_LocalMapDBData, location_id) == 0x44, "location_id at 0x44 (descriptor-proven)");

}  // namespace wh::guimodule
