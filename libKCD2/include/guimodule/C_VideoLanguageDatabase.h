#pragma once
#include <cstdint>
#include "databasemodule/C_ObjectTableDatabase.h"
#include "guimodule/S_VideoLanguage.h"
#include "guimodule/S_VideoLanguageDBData.h"

// -----------------------------------------------
// wh::guimodule::C_VideoLanguageDatabase -- the "video_language2audio_track" table
// (KCD2 1.5.6, kd7u).  sizeof 0x98 (template extent -- global static).
// -----------------------------------------------
// RTTI TD RVA 0x4CAF0F0, COL RVA 0x41390C8, vtable 0x183C403B8. Stack: plain
// C_ObjectTableDatabase<S_VideoLanguage, S_VideoLanguageDBData>.
// GLOBAL STATIC @0x1853269F0 (thunk sub_181945CD4; table "video_language2audio_track",
// group "ui"). Register [7] sub_181942F28 (descs qword_1854B1FE0, builder sub_180172890,
// 3 cols, stride 0x0C); ParseRows [10] sub_181647580 (push sub_182420E78, grow
// sub_18098A0E4); post-load [16] stub sub_18041A6A0.

namespace wh::guimodule {

class C_VideoLanguageDatabase
    : public wh::databasemodule::C_ObjectTableDatabase<S_VideoLanguage, S_VideoLanguageDBData>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_VideoLanguageDatabase;
};
static_assert(sizeof(C_VideoLanguageDatabase) == 0x98, "C_VideoLanguageDatabase must be 0x98 (OTD extent)");

}  // namespace wh::guimodule
