#pragma once

// -----------------------------------------------
// IMovieListener -- KCD2 binary vtable order (kd7u)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6. RTTI: .?AUIMovieListener@@ (global namespace; CryCommon
// IMovieSystem.h -- interfuscator-eligible; binary order shown). Same 2-slot shape as
// the KCD1 replica.
//
// Shape evidence: C_UISubtitles' IMovieListener subobject (@+0x68, COL 0x4136210,
// vtable 0x183C33560) = 2 slots: [0] deleting-dtor adjustor thunk 0x18213A554
// (-0x68 -> 0x182B8C08C), [1] 0x1807EE2E8 = OnMovieEvent(event, pSequence): for
// sequences with the vf[+0x240] flag set, event 0 (Started) registers the subtitle
// sink at *(ctx+0x1A0)+0x2F0 and events 1/2 (Stopped/Aborted) clear it.
// Registration: C_UISubtitles::Init calls IMovieSystem (qword_18492D8F0) vf[+0xC8]
// AddMovieListener(nullptr = all sequences, this+0x68).
//
// [KCD1 parity note] In KCD1 the movie listener lived on C_UIEHud and drove the
// RatioStrips letterbox reveal for NoHUD cutscenes; in KCD2 the movie listener lives
// on C_UISubtitles and only routes SUBTITLES -- cutscene letterboxing now flows
// through the source-monitor -> C_UIHudMask "SetRatioPreset" path instead.

namespace Offsets {

struct IMovieListener {
    virtual ~IMovieListener() = default;                                  // [0]
    virtual void OnMovieEvent(int movieEvent, void* pAnimSequence) = 0;   // [1] 0x1807EE2E8 (C_UISubtitles impl); IAnimSequence*
};
static_assert(sizeof(IMovieListener) == 8);

}  // namespace Offsets
