#pragma once
#include <cstdint>
#include "CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::guimodule::S_MapLabel -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// Parsed "ui_map_label" Data element (NON-polymorphic), stored in C_MapLabelDatabase's
// rows vector. Data offsets == Row offsets (inline converter in ParseRows sub_180D0FFD4,
// string -> CryStringT; in-place push sub_182B24A1C advances +0x30).

namespace wh::guimodule {

struct S_MapLabel {
    CryGUID          ui_map_label_id;   // +0x00
    CryGUID          location_id;       // +0x10
    float            position_x;        // +0x20
    float            position_y;        // +0x24
    CryStringT<char> ui_name;           // +0x28
};
static_assert(sizeof(S_MapLabel) == 0x30, "S_MapLabel must be 0x30 (ParseRows sub_180D0FFD4)");

}  // namespace wh::guimodule
