#pragma once
#include <cstdint>

// -----------------------------------------------
// IFlashPlayer_RenderProxy — KCD2 binary vtable order (INTERFUSCATED)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). SDK: CryEngine CryCommon/IFlashPlayer.h
// (<interfuscator:shuffle> block).
// RTTI: .?AUIFlashPlayer_RenderProxy@@ (global namespace, struct)
// Impl: second base of CFlashPlayer at +0x08 -> vtable 0x183DC09B8 (5 slots;
//       KCD1 had 4 — GetPlayback inserted). RTTI COL 0x18418C048 this_off == 8;
//       slots [3]/[4] are `this`-adjust (-8) thunks onto CFlashPlayer methods.
//
// The render thread invokes these callbacks after CFlashPlayer::Render enqueued
// a shared_ptr<IFlashPlayer_RenderProxy> on the renderer (qword_18492D908
// vf+0x360). Each callback pushes the player's clear/stereo/canvas state
// (player +0x20 clearFlags, +0x24 clearColor, +0x34/+0x38 depth/parallax,
// +0x3C/+0x3D flag bits) into the process-global CScaleformRecording
// (player+0xE0, vtable 0x183DC0338) via its virtual setters vf+432..488,
// then draws (movie vf+0x130) or replays a command buffer.
//
// [KCD1 -> KCD2 deltas] releaseOnExit params are GONE from [0][1][2]: lifetime
// now rides the enqueued shared_ptr (no Release/refcount calls in any body).
// NEW slot [3] GetPlayback (CE5 API name; behavior verified).

struct IScaleformPlayback;   // global namespace, matches IFlashPlayer.h fwd decl

namespace Offsets {

struct IFlashPlayer_RenderProxy {
    enum EFrameType { EFT_Mono, EFT_StereoLeft, EFT_StereoRight };

    // Binary vtable 0x183DC09B8 (5 slots)
    virtual void RenderCallback(EFrameType ft) = 0;
        // [0] 0x00 0x1803FA7D4 — locks player SRWLOCK (+0xF8); skips if renderer
        //     query qword_18492D908 vf+0xA68(11) says blocked; pushes clearFlags/
        //     clearColor (recording vf+432), stereo frame (vf+448: ft!=0, ft==1),
        //     fixed-depth bit1 (vf+456), the 3 canvas/mask bits (vf+472/480/488);
        //     draws movie (+0xD0 vf+0x130) under global lock stru_185575640.
        //     NO releaseOnExit (shared_ptr era)
    virtual void RenderPlaybackLocklessCallback(int cbIdx, EFrameType ft,
                                                bool finalPlayback = true) = 0;
        // [1] 0x08 0x18244FD08 — command buffer element *(player+0x120) +
        //     0xB0*cbIdx (KCD1: +0x120 stride 0x38); error 200 "Command buffer was not
        //     initialized!" if null; same recording flag push; executes via
        //     renderer vf+0x7B0(qword_18492D908, GetPlayback(), elem);
        //     finalPlayback -> sub_18244FF70 (buffer element reset).
        //     NO releaseOnExit
    virtual void DummyRenderCallback(EFrameType ft) = 0;
        // [2] 0x10 0x1803B6E80 — nullsub (true no-op)
    virtual IScaleformPlayback* GetPlayback() = 0;
        // [3] 0x18 0x1821388FC — KCD2-NEW (CE5 API name): thunk (this-8) onto
        //     CFlashPlayer::GetPlayback 0x18056B14C == player slot [62]
    virtual ~IFlashPlayer_RenderProxy() {}
        // [4] 0x20 0x182138848 — thunk (this-8): scalar deleting dtor on player
        //     (0x18089184C); destruction normally via shared_ptr control block
};

}  // namespace Offsets
