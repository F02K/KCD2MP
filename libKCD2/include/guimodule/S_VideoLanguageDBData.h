#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_VideoLanguageDBData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x0C.
// -----------------------------------------------
// Raw "video_language2audio_track" table row. Columns from the C_VideoLanguageDatabase
// Register descriptor builder sub_180172890 (3 int cols, row stride 0x0C via
// sub_181942F28). The Data type (S_VideoLanguage) is a verbatim copy of this row.

namespace wh::guimodule {

struct S_VideoLanguageDBData {
    int32_t language_id;         // +0x00  int32, PK
    int32_t audio_track_id;      // +0x04
    int32_t subtitles_track_id;  // +0x08
};
static_assert(sizeof(S_VideoLanguageDBData) == 0x0C, "video_language2audio_track row stride is 0x0C");

}  // namespace wh::guimodule
