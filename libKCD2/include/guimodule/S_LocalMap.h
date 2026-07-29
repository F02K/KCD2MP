#pragma once
#include <cstdint>
#include "CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::guimodule::S_LocalMap -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (PACKED).
// -----------------------------------------------
// Parsed "ui_local_maps" Data element (NON-polymorphic), stored in C_LocalMapDatabase's
// rows vector. Data offsets == Row offsets (converter sub_181507BAC copies
// field-for-field, string -> CryStringT; push sub_181507AEC advances +0x60).
// location_id stays at +0x44 -> pack(4) like the row.

namespace wh::guimodule {

#pragma pack(push, 4)
struct S_LocalMap {
    CryGUID          ui_local_map_id;          // +0x00
    CryStringT<char> ui_local_map_name;        // +0x10
    float            position_x;               // +0x18
    float            position_y;               // +0x1C
    float            width;                    // +0x20
    float            height;                   // +0x24
    float            active_x;                 // +0x28
    float            active_y;                 // +0x2C
    float            active_width;             // +0x30
    float            active_height;            // +0x34
    bool             active_area_ellipse;      // +0x38
    uint8_t          _pad39[3];                // +0x39
    int32_t          tile_width;               // +0x3C
    int32_t          tile_height;              // +0x40
    CryGUID          location_id;              // +0x44
    bool             enabled_by_default;       // +0x54
    bool             enabled_only_when_inside; // +0x55
    uint8_t          _pad56[2];                // +0x56
    int32_t          priority;                 // +0x58
    uint8_t          _pad5C[4];                // +0x5C
};
#pragma pack(pop)
static_assert(sizeof(S_LocalMap) == 0x60, "S_LocalMap must be 0x60 (converter sub_181507BAC)");
static_assert(offsetof(S_LocalMap, location_id) == 0x44, "location_id at 0x44");

}  // namespace wh::guimodule
