#pragma once
#include <cstdint>
#include <functional>
#include "guimodule/C_UIFlashBase.h"
#include "dialogmodule/I_UISubtitles.h"
#include "Offsets/vtables/ISubtitleHandler.h"
#include "Offsets/vtables/IMovieListener.h"

// -----------------------------------------------
// wh::guimodule::C_UISubtitles -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xC8.
// -----------------------------------------------
// RTTI .?AVC_UISubtitles@guimodule@wh@@ (TD 0x184CD0910).
// Bases: C_UIFlashBase @+0x00 (COL 0x4136170), IUIElementEventListener @+0x10
// (COL 0x4136198), dialogmodule::I_UISubtitles @+0x58 (COL 0x41361C0),
// ISubtitleHandler @+0x60 (COL 0x41361E8), IMovieListener @+0x68 (COL 0x4136210).
// vtables: primary 0x183C33618 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183C335D0 (8), subtitles 0x183C33598 (6), handler 0x183C33578 (3),
// movie 0x183C33560 (2). ctor sub_181886B24; rttr as_std_shared_ptr creator
// sub_181886A98 (alloc 0xC8) -- shared_ptr-managed like every module screen
// (_Ref_count<C_UISubtitles> TD 0x184CC63E0; the creator adopts via sub_181886B88).
//
// Subtitle HUD. Init [1] sub_180C3C158: base Init, caches the wh_ui_SubtitlesEnabled
// ICVar (+0x70), installs change-callbacks on wh_ui_EnableSubtitleSpeakerName /
// wh_ui_EnableSubtitleBg / wh_ui_SubtitleSize (sub_182B8E1F8/sub_182B8E19C/
// sub_182B8E83C), publishes the I_UISubtitles subobject into the dialogmodule object
// (*(*(ctx+0x150)+0x10)+0x120 = this+0x58) and registers the IMovieListener with
// IMovieSystem (qword_18492D8F0 vf[+0xC8](nullptr, this+0x68) = all sequences).
// OnModuleMessage [3] sub_18085E398: msg 52 pushes the three CVars to flash
// ("SetSubtitleSpeakerNameEnable"/"SetSubtitleBgEnable"/"SetSubtitleSize"); msg 76
// arms both countdowns and captures a completion std::function (sub_1803E23C0).
// Update [4] sub_1811795B4: m_narrativeCountdown -> flash "HideNarrativeSubtitles";
// m_faderCountdown -> flash "SetFaderState"(10) + invoke/reset m_pendingFadeCallback.
// ISubtitleHandler::ShowSubtitle [H1] 0x182B8F160: crc32s the localization key;
// show -> I_UISubtitles::ShowSubtitle(key,"",0,0,1) + cache crc; hide (matching crc)
// -> ClearSubtitles. IMovieListener::OnMovieEvent [V1] 0x1807EE2E8: for flagged
// sequences (seq vf[+0x240]) registers/unregisters the subtitle sink at
// *(ctx+0x1A0)+0x2F0 on Started/Stopped.
//
// [FUNDAMENTAL vs KCD1] KCD1's global movie listener lived on C_UIEHud and drove the
// letterbox; the KCD2 movie listener here only routes cutscene SUBTITLES -- the
// letterbox moved to C_UIHudMask/"SetRatioPreset".

struct ICVar;

namespace wh::guimodule {

class C_UISubtitles : public C_UIFlashBase,
                      public wh::dialogmodule::I_UISubtitles,
                      public Offsets::ISubtitleHandler,
                      public Offsets::IMovieListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UISubtitles;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x180C3C158
    void OnModuleMessage(void* msg) override;   // [3] 0x18085E398
    void Update() override;                     // [4] 0x1811795B4
    // I_UISubtitles overrides.
    void ShowSubtitle(const CryStringT<char>& text, const CryStringT<char>& speaker,
                      bool a4, bool bForce, bool a6) override;   // [T1] 0x181457948
    void ClearSubtitles() override;                              // [T2] 0x1817990CC
    // ISubtitleHandler override.
    void ShowSubtitle(const char* szKey, bool bShow) override;   // [H1] 0x182B8F160
    // IMovieListener override.
    void OnMovieEvent(int movieEvent, void* pAnimSequence) override;   // [V1] 0x1807EE2E8

    ICVar*   m_pSubtitlesEnabledCVar;        // +0x70  wh_ui_SubtitlesEnabled (Init @0x180c3c17d)
    uint32_t m_lastSubtitleCrc;              // +0x78  crc32 of the active handler subtitle key (@0x182b8f1ed)
    uint8_t  _pad7C[4];                      // +0x7C
    std::function<void()> m_pendingFadeCallback;  // +0x80  captured by msg 76 (sub_1803E23C0), fired by Update then reset
    float    m_faderCountdown;               // +0xC0  ctor -1.0f; Update -> "SetFaderState"(10) at <= 0
    float    m_narrativeCountdown;           // +0xC4  ctor -1.0f; Update -> "HideNarrativeSubtitles" at <= 0
};
static_assert(sizeof(C_UISubtitles) == 0xC8, "C_UISubtitles must be 0xC8");
static_assert(offsetof(C_UISubtitles, m_pendingFadeCallback) == 0x80, "fade callback at 0x80");
static_assert(offsetof(C_UISubtitles, m_faderCountdown) == 0xC0, "countdowns at 0xC0");

}  // namespace wh::guimodule
