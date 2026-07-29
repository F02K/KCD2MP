#pragma once
#include "Scaleform/GRefCountImplCore.h"

// -----------------------------------------------
// GFxMovieView -- Scaleform GFx movie instance interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Statically linked Scaleform GFx 3.x; no RTTI, SDK-correlated name (folds the
// SDK's GFxMovie + GFxMovieView into the one 73-slot vtable the binary has).
// Concrete instance: GFxMovieRoot, primary vtable 0x183DA0CB0 (73 slots,
// 0x183DA0CB0..0x183DA0EF0, extent VERIFIED by slot scan -- 0x183DA0EF8 is the
// next vtable). CFlashPlayer (see CryEngine/CryCommon/CFlashPlayer.h) wraps
// exactly this interface: nearly every CFlashPlayer vfunc takes the SRWLOCK at
// player+0xF8 and forwards to the GFxMovieView* at player+0xD0.
//
// Slot IDENTITIES below come from the CFlashPlayer wrapper bodies (each
// Offsets::IFlashPlayer slot comment names the GFx vfunc byte offset it
// forwards to -- see Offsets/vtables/IFlashPlayer.h). Only Advance [37] has a
// fully verified SIGNATURE (decompile of its implementation 0x181CA4D90); the
// other identified slots keep placeholder signatures on purpose -- their GFx
// argument shapes (GFxValue&, GFxEvent&, ...) are not RE'd yet, and calling
// through a guessed signature would be worse than not calling at all. Reach
// them through CFlashPlayer/IFlashPlayer instead.

class GFxMovieView : public GRefCountImplCore {
public:
    // GRefCountImplCore::~ occupies [0] (0x181CA2820 scalar deleting dtor -> base dtor 0x181CA13B0;
    // the base dtor FREES any still-pending load queue without loading it).
    virtual void  _vf1() = 0;                                    // [1]  0x008  (CreateInstance 0x181C622D0 feeds its result to 0x181D370D0)
    virtual void  _vf2() = 0;                                    // [2]  0x010  GetCurrentFrame
    virtual void  _vf3() = 0;                                    // [3]  0x018  HasLooped
    virtual void  _vf4() = 0;                                    // [4]  0x020  GotoFrame
    virtual void  _vf5() = 0;                                    // [5]  0x028  GotoLabeledFrame
    virtual void  _vf6() = 0;                                    // [6]  0x030
    virtual void  _vf7() = 0;                                    // [7]  0x038
    virtual void  _vf8() = 0;                                    // [8]  0x040  SetVisible
    virtual void  _vf9() = 0;                                    // [9]  0x048  IsVisible
    virtual void  _vf10() = 0;                                   // [10] 0x050  VariableExists
    virtual void  _vf11() = 0;                                   // [11] 0x058  CreateString
    virtual void  _vf12() = 0;                                   // [12] 0x060  CreateStringW
    virtual void  _vf13() = 0;                                   // [13] 0x068  CreateObject
    virtual void  _vf14() = 0;                                   // [14] 0x070  CreateArray
    virtual void  _vf15() = 0;                                   // [15] 0x078  CreateFunction
    virtual void  _vf16() = 0;                                   // [16] 0x080  SetVariable
    virtual void  _vf17() = 0;                                   // [17] 0x088  GetVariable
    virtual void  _vf18() = 0;                                   // [18] 0x090  SetVariableArray
    virtual void  _vf19() = 0;                                   // [19] 0x098
    virtual void  _vf20() = 0;                                   // [20] 0x0A0  GetVariableArraySize
    virtual void  _vf21() = 0;                                   // [21] 0x0A8  GetVariableArray
    virtual void  _vf22() = 0;                                   // [22] 0x0B0
    virtual void  _vf23() = 0;                                   // [23] 0x0B8  Invoke
    virtual void  _vf24() = 0;                                   // [24] 0x0C0
    virtual void  _vf25() = 0;                                   // [25] 0x0C8  SetViewport
    virtual void  _vf26() = 0;                                   // [26] 0x0D0  GetViewport
    virtual void  _vf27() = 0;                                   // [27] 0x0D8  SetViewScaleMode
    virtual void  _vf28() = 0;                                   // [28] 0x0E0  GetViewScaleMode
    virtual void  _vf29() = 0;                                   // [29] 0x0E8  SetViewAlignment
    virtual void  _vf30() = 0;                                   // [30] 0x0F0  GetViewAlignment
    virtual void  _vf31() = 0;                                   // [31] 0x0F8
    virtual void  _vf32() = 0;                                   // [32] 0x100
    virtual void  _vf33() = 0;                                   // [33] 0x108
    virtual void  _vf34() = 0;                                   // [34] 0x110
    virtual void  _vf35() = 0;                                   // [35] 0x118
    virtual void  _vf36() = 0;                                   // [36] 0x120  Restart
    // Ticks the timeline; runs queued AS actions and (all 3 exit paths) drains
    // the load queue: 0x181CA7950 -> 0x181CAFCB0 -> ProcessLoadQueue(this).
    // Return = seconds until the next frame is due (GFx-3 Advance contract).
    virtual float Advance(float deltaTime, int frameCatchUpCount) = 0;  // [37] 0x128  0x181CA4D90  VERIFIED
    virtual void  _vf38() = 0;                                   // [38] 0x130
    virtual void  _vf39() = 0;                                   // [39] 0x138
    virtual void  _vf40() = 0;                                   // [40] 0x140  SetPause
    virtual void  _vf41() = 0;                                   // [41] 0x148  IsPaused
    virtual void  _vf42() = 0;                                   // [42] 0x150  SetBackgroundColor
    virtual void  _vf43() = 0;                                   // [43] 0x158  SetBackgroundAlpha
    virtual void  _vf44() = 0;                                   // [44] 0x160  GetBackgroundAlpha
    virtual void  _vf45() = 0;                                   // [45] 0x168  HandleEvent
    virtual void  _vf46() = 0;                                   // [46] 0x170
    virtual void  _vf47() = 0;                                   // [47] 0x178
    virtual void  _vf48() = 0;                                   // [48] 0x180  HitTest
    virtual void  _vf49() = 0;                                   // [49] 0x188
    virtual void  _vf50() = 0;                                   // [50] 0x190
    virtual void  _vf51() = 0;                                   // [51] 0x198
    virtual void  _vf52() = 0;                                   // [52] 0x1A0
    virtual void  _vf53() = 0;                                   // [53] 0x1A8
    virtual void  _vf54() = 0;                                   // [54] 0x1B0
    virtual void  _vf55() = 0;                                   // [55] 0x1B8
    virtual void  _vf56() = 0;                                   // [56] 0x1C0
    virtual void  _vf57() = 0;                                   // [57] 0x1C8
    virtual void  _vf58() = 0;                                   // [58] 0x1D0
    virtual void  _vf59() = 0;                                   // [59] 0x1D8
    virtual void  _vf60() = 0;                                   // [60] 0x1E0
    virtual void  _vf61() = 0;                                   // [61] 0x1E8
    virtual void  _vf62() = 0;                                   // [62] 0x1F0
    virtual void  _vf63() = 0;                                   // [63] 0x1F8
    virtual void  _vf64() = 0;                                   // [64] 0x200
    virtual void  _vf65() = 0;                                   // [65] 0x208
    virtual void  _vf66() = 0;                                   // [66] 0x210
    virtual void  _vf67() = 0;                                   // [67] 0x218
    virtual void  _vf68() = 0;                                   // [68] 0x220
    virtual void  _vf69() = 0;                                   // [69] 0x228  (CreateInstance calls it on the half-built root when task setup fails -- destroy/release path)
    virtual void  _vf70() = 0;                                   // [70] 0x230
    virtual void  _vf71() = 0;                                   // [71] 0x238
    virtual void  _vf72() = 0;                                   // [72] 0x240
};
static_assert(sizeof(GFxMovieView) == 0x10, "vfptr + GRefCountImplCore payload");
