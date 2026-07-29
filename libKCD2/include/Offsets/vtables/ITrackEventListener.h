#pragma once

// -----------------------------------------------
// ITrackEventListener -- KCD2 binary vtable order (kd7u)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6. RTTI: .?AUITrackEventListener@@ (global namespace; CryCommon
// IMovieSystem.h -- interfuscator-eligible; binary order shown).
//
// Shape evidence: C_TrackViewBase's subobject @+0x68 (COL 0x4138D30, vtable 0x183B2E208)
// = 3 slots: [0] deleting-dtor adjustor thunk 0x18213A39C, [1] OnTrackEvent 0x182B38134
// (trackview event track callback), [2] nullsub 0x1803B6E80 (empty default).
// Param shapes UNVERIFIED (SDK: OnTrackEvent(ITrackEvent&)-style; kept opaque).

namespace Offsets {

struct ITrackEventListener {
    virtual ~ITrackEventListener() = default;         // [0]
    virtual void OnTrackEvent(void* pTrackEvent) = 0; // [1] 0x182B38134 (C_TrackViewBase impl); param shape UNVERIFIED
    virtual void _vf2();                              // [2] nullsub default [role UNVERIFIED]
};
static_assert(sizeof(ITrackEventListener) == 8);

}  // namespace Offsets
