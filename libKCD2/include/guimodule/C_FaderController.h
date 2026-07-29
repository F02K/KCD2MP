#pragma once
#include <cstdint>
#include <memory>
#include "guimodule/I_FaderController.h"
#include "Offsets/vtables/IMovieListener.h"
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_FaderController -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x220 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184CB09C8; COLs +0x00/0x41390F0, +0x08/0x4139118. vtables: primary
// (I_FaderController) 0x183A97220 / IMovieListener@+0x08 0x183A96D88.
// Bases: I_FaderController@+0x00 + IMovieListener@+0x08 + enable_shared_from_this
// @+0x10 (+ empty CryDeferrable/ICryDeferrable @+0x10 per CHD -- not modeled; the
// related C_BasicFader<C_FaderController> TD is NOT in the CHD base array, do not add).
// ctor sub_180D26844(this, owner); dtor sub_182B35514; make_shared creator sub_1818ACD14
// (alloc 0x220, object at ctrl+0x10 @0x1818acd8d). Module-owned shared_ptr at
// C_GUIModule+0x58 (spine.md).
//
// The screen-fade controller: fades keyed by 64-bit FNV-1a ids in the +0xC8 (pending) /
// +0x108 (active) maps; IMovieListener registered via IMovieSystem AddMovieListener
// (qword_18492D8F0 vf[+0xC8], @ctor 0x180d26ba5). Registers 11 wh_ui_Fader* CVars
// (Debug/Profiler/Suspend/DebugAlpha/AutoDebug/FadeOutDefaultDuration/
// FadeInDefaultDuration/ProgressIndicatorTimerDuration/DebugStuckDetectorTimerDuration/
// ProfilerSaveDuration/ProfilerSavePath). Container internals partially typed --
// opaque blocks below carry the ctor anchors [G4c §B7, UNVERIFIED 6].

namespace wh::guimodule {

class C_FaderController : public I_FaderController,                       // +0x00
                          public Offsets::IMovieListener,                 // +0x08
                          public std::enable_shared_from_this<C_FaderController> {  // +0x10 (0x10)
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FaderController;
    // I_FaderController impls (vtable 0x183A97220).
    void _vf1() override;                              // [1] 0x180C08E84  appends default duration from +0x1B4
    void RequestFade(uint64_t fadeId, int a3) override;// [2] 0x180C09498  pending->active map move, sets +0x170
    void _vf3() override;                              // [3] 0x180C09488
    void CancelFade(uint64_t fadeId) override;         // [4] 0x180C09710  remove from active map

    // IMovieListener (vtable 0x183A96D88).
    void OnMovieEvent(int movieEvent, void* pAnimSequence) override;   // [1] 0x1807EE344

    wh::shared::C_Signal<> m_signal20;   // +0x20  delegate table unk_1856679D8 [signature UNVERIFIED]
    C_GUIModule* m_pOwner;               // +0x30  owning module (creator sub_1818ACD14 a2 = C_GUIModule 'this'; module stores the shared_ptr at C_GUIModule+0x58)
    uint8_t  m_flags38[0x10];            // +0x38  word/byte/dword flag block [roles UNVERIFIED]
    uint8_t  m_map48[0x40];              // +0x48  sub_1806B5C70 map/obj [type UNVERIFIED]
    uint8_t  m_map88[0x40];              // +0x88  sub_1806B5C70 map/obj [type UNVERIFIED]
    uint8_t  m_pendingFades[0x40];       // +0xC8  sub_18043C878 -- fade map keyed by FNV-1a id [type UNVERIFIED]
    uint8_t  m_activeFades[0x40];        // +0x108 sub_18043C878 -- fade map keyed by FNV-1a id [type UNVERIFIED]
    void*    m_srwLock;                  // +0x148  SRWLOCK.Ptr -- recursive RW lock (InitializeSRWLock @sub_180D26214)
    uint32_t m_lockRecursion;            // +0x150  recursion depth (sub_1803D902C)
    uint32_t m_lockOwnerThreadId;        // +0x154  owning thread id (GetCurrentThreadId)
    std::vector<std::function<void()>> m_callbacks158;  // +0x158  0x40-byte std::function elems (elem dtor sub_181501528) [functor signature UNVERIFIED]
    int32_t  m_requestArg170;            // +0x170  RequestFade stores its 2nd arg here (0x180C09527)
    uint8_t  _pad174[4];                 // +0x174
    int64_t  m_timerSentinels[3];        // +0x178  ctor -100000 x3
    uint8_t  m_cvarBlock190[0x40];       // +0x190..+0x1D0  ICVar*/bound floats (+0x19C..+0x1C8) + +0x1B4 default duration [layout UNVERIFIED]
    uint8_t  m_placeholderFader[0x30];   // +0x1D0  "Placeholder" fader obj (sub_1808DD5C0) [type UNVERIFIED]
    uint64_t _unk200;                    // +0x200
    CryStringT<char> m_name;             // +0x208  ctor "fader"
    // +0x210: NOT a member. C_FaderController ends at +0x210 (last member m_name @+0x208).
    // The 0x220 make_shared block = 0x10 _Ref_count_obj2 control-block prefix + 0x210 object
    // (creator sub_1818ACD14: alloc 0x220, ctor sub_180D26844(v5+16), object @ alloc+0x10).
};
static_assert(sizeof(C_FaderController) == 0x210, "C_FaderController object is 0x210; the 0x220 make_shared block = 0x10 _Ref_count_obj2 control prefix + 0x210 object (creator sub_1818ACD14, ctor at alloc+0x10)");
static_assert(offsetof(C_FaderController, m_timerSentinels) == 0x178, "timer sentinels at 0x178");
static_assert(offsetof(C_FaderController, m_name) == 0x208, "name at 0x208");

}  // namespace wh::guimodule
