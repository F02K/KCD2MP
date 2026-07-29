#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_VideoLanguage -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x0C.
// -----------------------------------------------
// Parsed "video_language2audio_track" Data element (NON-polymorphic) -- identical to its
// row (ParseRows sub_181647580 verbatim 3-dword copy; in-place push sub_182420E78
// advances +0x0C).

namespace wh::guimodule {

struct S_VideoLanguage {
    int32_t language_id;         // +0x00
    int32_t audio_track_id;      // +0x04
    int32_t subtitles_track_id;  // +0x08
};
static_assert(sizeof(S_VideoLanguage) == 0x0C, "S_VideoLanguage must be 0x0C");

}  // namespace wh::guimodule
