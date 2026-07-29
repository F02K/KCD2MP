#pragma once
#include <cstdint>
#include <memory>
#include "guimodule/C_UIFlashBase.h"
#include "framework/CryDeferrable.h"
#include "Offsets/vtables/ISystem.h"   // Offsets::ISystemEventListener

// -----------------------------------------------
// wh::guimodule::C_UILoadingScreen -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88.
// -----------------------------------------------
// RTTI .?AVC_UILoadingScreen@guimodule@wh@@ (TD 0x184CE24F8).
// Bases: C_UIFlashBase @+0x00 (COL 0x4388760), IUIElementEventListener @+0x10
// (COL 0x4388738), ISystemEventListener @+0x58 (COL 0x43886E8), UnsafeOp::
// CryDeferrable<0> @+0x60 (EMPTY -- overlaps m_60, same EBO family as
// C_UIFullUIModeHelper). vtables: primary 0x183EF7C30 (14; GetElementName [11]
// 0x181A719B0 = "LoadingScreen" -- own flash movie, not "hud"), listener 0x183EF7CC8
// (8), system 0x183EF7CA8 (3: [0] dtor thunk, [1] nullsub OnSystemEventAnyThread,
// [2] 0x1808AE710 OnSystemEvent). ctor sub_18187663C (also registers the system-event
// listener: ISystem (qword_18492D8C0) vf[+0x2A8] dispatcher ->vf[+8](this+0x58,
// "C_LoadingScreen", 1)); rttr as_std_shared_ptr creator sub_18187651C (alloc 0x88).
//
// Loading screen + loading-video screen. Init [1] sub_1807CF684: base Init, fetches
// the module fader by name (module+0x58 FaderController -> sub_1807CF9A0(...,
// "LoadingScreen")) into m_pLoadingFader, binds the "loading" action-map actions
// "loading_video_esc"(+"loading_video_skip") to skip lambdas. Deinit [2] sub_182BB911C
// unbinds both actions + a ctx+0xE8 list entry. Update [4] sub_181162C7C: 4-second
// video-elapsed check (ITimer qword_18492D880 vf[+0x40]) -> sub_181162D00 + m_bSkippable,
// and fires the scheduled fader op (sentinel int64 -100000 @+0x80) via sub_18288704C.
// OnSystemEvent [2] 0x1808AE710: reacts to system events 19/20/22/38 (level load /
// map-mission chain; skipped in editor vf[+0x6C8]).

namespace wh::guimodule {

class C_FaderController;
template <typename T> class C_BasicFader;   // 0x30 fader worker (vftable + CryStringT@+0x10); no full RE yet

class C_UILoadingScreen : public C_UIFlashBase,
                          public Offsets::ISystemEventListener,
                          public UnsafeOp::CryDeferrable<0> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UILoadingScreen;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x1807CF684
    void Deinit() override;                     // [2] 0x182BB911C
    void Update() override;                     // [4] 0x181162C7C
    const char* GetElementName() const override;   // [11] 0x181A719B0 = "LoadingScreen"
    // ISystemEventListener overrides (subobject +0x58).
    void OnSystemEventAnyThread(uint32_t event, uint64_t wparam, uint64_t lparam) override;   // [S1] nullsub
    void OnSystemEvent(uint32_t event, uint64_t wparam, uint64_t lparam) override;            // [S2] 0x1808AE710

    int32_t m_60;               // +0x60  ctor 0 [role UNVERIFIED; address shared with the empty CryDeferrable base]
    bool    m_flag64;           // +0x64  ctor 0 [role UNVERIFIED]
    bool    m_bSkippable;       // +0x65  ctor 1; Update sets 1 after the 4s window (@0x181162cbc) [name coined]
    uint8_t _pad66[2];          // +0x66
    float   m_videoStartTime;   // +0x68  Update elapsed base (@0x181162c98) [writer not traced]
    uint8_t _pad6C[4];          // +0x6C
    std::unique_ptr<C_BasicFader<C_FaderController>> m_pLoadingFader;   // +0x70  "LoadingScreen" fader op (sub_1808DD5C0 vtable); Init move-assign sub_1807CF820, deleting-dtor release sub_18194B6E0(p,1)
    int32_t m_78;               // +0x78  ctor 0 [role UNVERIFIED]
    bool    m_flag7C;           // +0x7C  ctor 0 [role UNVERIFIED]
    uint8_t _pad7D[3];          // +0x7D
    int64_t m_scheduledFadeTime;  // +0x80  ctor -100000 sentinel (sub_1808DDC44); checked by sub_1809D836C
};
static_assert(sizeof(C_UILoadingScreen) == 0x88, "C_UILoadingScreen must be 0x88");
static_assert(offsetof(C_UILoadingScreen, m_pLoadingFader) == 0x70, "fader at 0x70");
static_assert(offsetof(C_UILoadingScreen, m_scheduledFadeTime) == 0x80, "schedule sentinel at 0x80");

}  // namespace wh::guimodule
