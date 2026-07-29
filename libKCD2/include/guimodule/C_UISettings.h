#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIMenuBase.h"
#include "guimodule/C_UIKeybinds.h"
#include "guimodule/C_UIPhotomodeMenu.h"
#include "framework/C_ReadinessObserver.h"
#include "Offsets/vtables/IPlayerProfileListener.h"
#include "Offsets/vtables/ISystem.h"   // Offsets::ISystemEventListener

struct ICVar;

// -----------------------------------------------
// wh::guimodule::C_UISettings -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x490.
// -----------------------------------------------
// RTTI .?AVC_UISettings@guimodule@wh@@ (TD 0x184CD9BD0). ctor sub_180600544 (in-place).
// Bases: C_UIMenuBase @+0x00 (binds the shared "Menu" movie), IUIElementEventListener
// @+0x10, IPlayerProfileListener @+0x58 (registered with the profile manager in the
// ctor @0x18060107e), ISystemEventListener @+0x60 (registered as "C_UISettings" in
// Init). vtables: primary 0x183EF5B80 ([1] Init 0x180612600, [2] Deinit 0x182BA718C,
// [4] Update 0x1810C31E4, [11] GetElementName 0x181A8F230 "Menu"), profile listener
// ([1] 0x181A71808 -> sub_18079CF88, [2] 0x180A6A4D8 -> sub_18079C4A8), system
// listener (OnSystemEvent 0x181F8D320).
// Lives EMBEDDED in C_UIMenu at +0x1D8 (ctor call @0x1805ff438; a2 = owner backptr).
//
// The settings controller: owns every settings-page builder (E_MenuPage 11..16 + 20),
// the ICVar cache the pages read/write, the "current values" snapshots used by the
// Apply/Reset/IsSomeButtonChanged flow, the resolution/refresh-rate mode lists, the
// s_*Volume proxy cvars, and the embedded keybinds/photomode sub-screens. Also
// registers the consent CVar block (wh_ui_eulaConfirmedVersion, wh_ui_eulaCurrentVersion,
// wh_ui_telemetryConfirmed, wh_ui_consoleModeChosen, wh_ui_dlcOverlayShown[Owned]) --
// value-less registrations, no members.
//
// Init 0x180612600: base Init; Init on m_keybinds/m_photomode; inbound flash
// dispatchers "OnOpenWebPage" -> 0x182BA88EC, "OnOpenWebPageSocial" -> 0x182BA8A18,
// "OnInteractiveValue" -> 0x182BA8718, "OnInteractiveChoice" -> 0x181F8CDC0.
// Update 0x1810C31E4: base drain + m_keybinds.Update + auto-clears busy protection
// (0x181F8C270) one second after m_busyProtectionTime.
// OnSystemEvent 0x181F8D320: 46 -> audio refresh; 4122 (display changed) -> rebuild
// resolution+refresh lists; 4123 -> push last (largest) mode into r_Width/r_Height and
// re-derive m_resolutionIndex; both tails re-sync sys_MaxFPS from the VSync choice.
// The ctor also installs an OnChange functor (sub_181F800A0) on r_Width/r_Height and
// registers m_rtpcVolume* as engine-side RTPC write targets ("volume_master"/"_music"/
// "_sfx").
//
// [FUNDAMENTAL vs KCD1] KCD1's CUISettings was a CUIManager-registered event system;
// KCD2 demotes it to a value member of C_UIMenu and moves the widget protocol into the
// shared "Menu" movie (C_UIMenuBase builder tier).

namespace wh::guimodule {

class C_UIMenu;

class C_UISettings
    : public C_UIMenuBase
    , public Offsets::IPlayerProfileListener
    , public Offsets::ISystemEventListener
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UISettings;

    struct S_Resolution {
        uint32_t m_width;    // +0x00
        uint32_t m_height;   // +0x04
    };

    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x180612600
    void Deinit() override;                     // [2] 0x182BA718C
    void Update() override;                     // [4] 0x1810C31E4
    // IPlayerProfileListener @+0x58 (which body saves vs loads INFERRED from SDK slot order).
    void SaveToProfile(Offsets::IPlayerProfile* pProfile, bool online, unsigned int reason) override;    // [P1] 0x181A71808 -> sub_18079CF88
    void LoadFromProfile(Offsets::IPlayerProfile* pProfile, bool online, unsigned int reason) override;  // [P2] 0x180A6A4D8 -> sub_18079C4A8
    // ISystemEventListener @+0x60.
    void _vf0() override;                                                                // [S0] dtor-region thunk
    void OnSystemEventAnyThread(uint32_t event, uint64_t wparam, uint64_t lparam) override;  // [S1]
    void OnSystemEvent(uint32_t event, uint64_t wparam, uint64_t lparam) override;           // [S2] 0x181F8D320

    // --- Native methods (RVA forwarders in src/guimodule/C_UISettings.cpp) ---
    // Settings-page builders (each: m_pMenu->PreparePage(N) -> widgets -> footer
    // handlers Reset/Apply/Back -> m_pMenu->ShowPage(); widget sets documented in
    // analysis/ui_survey/menu_page_builders.md §2).
    void BuildGameSettingsPage();       // 0x180F6AD04  page 11 (languages/subtitles/telemetry)
    void BuildGraphicSettingsPage();    // 0x180F6C288  page 12 (resolution/window/vsync/FOV/upscaling/HDR)
    void BuildAdvancedGraphicsPage();   // 0x180F6B40C  page 13 (AA + 11 detail categories)
    void BuildSoundSettingsPage();      // 0x180F6A0E0  page 14 (volume sliders 60/61/62)
    void BuildControlsPage();           // 0x180F6A51C  page 15 (mouse/controller groups)
    void BuildKeybindsPage();           // 0x180F68614  page 16 (S_KeybindsSuperaction rows)
    // RootPhotomode composite: PreparePage(20) -> flash "ShowRootMenu" ->
    // m_photomode.BuildCvarWidgets() -> ShowPage.
    void ShowRootPhotomodePage();       // 0x181F8DE50  page 20
    // Re-read every live value into the snapshot block below (ctor + page-sync entry).
    void SnapshotCurrentSettings();     // 0x180F69FC0
    // {master, music, sound} from the s_*Volume proxy cvars (GetFVal).
    float* GetVolumes(float (&out)[3]);            // 0x180F6A08C
    // Push three volume floats to the flash sliders (SetValue 60/61/62).
    void PushVolumesToFlash(const float (&vols)[3]);   // 0x180F69B5C
    // Index of (w, h) in m_resolutions -- exact match, else nearest by |dw|+|dh|.
    int FindResolutionIndex(uint32_t width, uint32_t height);   // 0x180F6D87C
    // Re-enumerate display modes (>= 1280x720, deduped, sorted) into m_resolutions
    // and refresh m_aspectRatioIndex.
    void RebuildResolutionList();       // 0x180F6D9CC
    // Re-enumerate refresh rates into m_refreshRates (fallback {30, 60}).
    void RebuildRefreshRateList();      // 0x181F8E080

    // IPlayerProfileListener vptr @+0x58, ISystemEventListener vptr @+0x60 (bases).

    // "Values at page open" snapshots, filled by SnapshotCurrentSettings and compared
    // by the Apply/Reset/IsSomeButtonChanged flow. Field breakdowns UNVERIFIED (one
    // sub-struct per settings page; the gatherers are the page-sync helpers).
    uint8_t  m_gameSettingsSnapshot[0x20];   // +0x068  sub_180F69D5C (page 11 values)
    uint8_t  m_controlsSnapshot[0x20];       // +0x088  sub_180F69E5C (page 15 values)
    uint8_t  m_graphicsSnapshot[0x40];       // +0x0A8  sub_180F6D2E8 (page 12 values)
    uint8_t  m_advGraphicsSnapshot[0x30];    // +0x0E8  sub_180F6D1EC (page 13 values)
    float    m_volumesSnapshot[3];           // +0x118  sub_180F6A08C {master, music, sound}
    uint8_t  _pad124[4];                     // +0x124

    // ICVar cache (ctor: wh_* created via sub_180B9512C, engine vars fetched via
    // IConsole::GetCVar vf+0xB8 with a "UISetting uses undefined CVar" dummy fallback
    // sub_182BA747C -- so the pointers are never null).
    ::ICVar* m_pCVarSubtitlesEnabled;          // +0x128  "wh_ui_SubtitlesEnabled" (default 1, VF_DUMPTODISK)
    ::ICVar* m_pCVarBubblesEnabled;            // +0x130  "wh_ui_BubblesEnabled"
    ::ICVar* m_pCVarTutorialsEnabled;          // +0x138  "wh_ui_TutorialsEnabled"
    ::ICVar* m_pCVarRoadMagnetism;             // +0x140  "wh_horse_RoadMagnetism" (fetched)
    ::ICVar* m_pCVarShowNormalCursor;          // +0x148  "wh_ui_ShowNormalCursor"
    ::ICVar* m_pCVarInputDevice;               // +0x150  "wh_ui_InputDevice" (1 kb+m / 2 xbox / 4 ps)
    ::ICVar* m_pCVarPreferredInputDevice;      // +0x158  "wh_ui_PreferredInputDevice"
    int32_t  m_unk160;                         // +0x160  ctor sub_1809EAEF8() (string->id via localization-ish lookup; semantic UNVERIFIED)
    int32_t  m_unk164;                         // +0x164  ctor sub_1819D55FC() (semantic UNVERIFIED)
    ::ICVar* m_pCVarSubtitleSpeakerName;       // +0x168  "wh_ui_EnableSubtitleSpeakerName"
    ::ICVar* m_pCVarSubtitleBg;                // +0x170  "wh_ui_EnableSubtitleBg"
    ::ICVar* m_pCVarSubtitleSize;              // +0x178  "wh_ui_SubtitleSize"
    ::ICVar* m_pCVarTelemetryConsent;          // +0x180  "wh_rpg_TelemetryMarketingConsent" (fetched)
    ::ICVar* m_pCVarAdaptiveHud;               // +0x188  "wh_ui_EnableAdaptiveHud"
    ::ICVar* m_pCVarWidth;                     // +0x190  "r_Width" (+ OnChange functor sub_181F800A0)
    ::ICVar* m_pCVarHeight;                    // +0x198  "r_Height" (+ OnChange functor)
    ::ICVar* m_pCVarSysSpec;                   // +0x1A0  "sys_spec"
    ::ICVar* m_pCVarSysSpecFull;               // +0x1A8  "sys_spec_full"
    ::ICVar* m_pCVarWindowType;                // +0x1B0  "r_WindowType"
    ::ICVar* m_pCVarShowFps;                   // +0x1B8  "wh_ui_fps"
    ::ICVar* m_pCVarVSync;                     // +0x1C0  "r_VSync"
    ::ICVar* m_pCVarMaxFPS;                    // +0x1C8  "sys_MaxFPS"
    ::ICVar* m_pCVarConsoleMode;               // +0x1D0  "wh_sys_ConsoleMode"
    ::ICVar* m_pCVarSuperResolutionMode;       // +0x1D8  "r_SuperResolution_Mode"
    ::ICVar* m_pCVarDlssPreset;                // +0x1E0  "r_SuperResolution_NVIDIA_DLSS_Preset"
    ::ICVar* m_pCVarDlssQuality;               // +0x1E8  "r_SuperResolution_NVIDIA_DLSS_QualityMode"
    ::ICVar* m_pCVarFsrQuality;                // +0x1F0  "r_SuperResolution_AMD_FSR_QualityMode"
    ::ICVar* m_pCVarXessQuality;               // +0x1F8  "r_SuperResolution_INTEL_XeSS_QualityMode"
    ::ICVar* m_pCVarSgsrQuality;               // +0x200  "r_SuperResolution_QCOM_SGSR_QualityMode"
    ::ICVar* m_pCVarSrSharpness;               // +0x208  "r_SuperResolution_Sharpness"
    ::ICVar* m_pCVarMotionBlur;                // +0x210  "r_MotionBlur"
    ::ICVar* m_pCVarNearDof;                   // +0x218  "wh_cl_NearDof"
    ::ICVar* m_pCVarHdrOutput;                 // +0x220  "r_HDROutput"
    ::ICVar* m_pCVarHdrUiBrightness;           // +0x228  "r_HDRUIBrightness"

    uint64_t m_unk230;                         // +0x230  written by sub_180F69698 from a {u64, int} param (audio-output change data; semantic UNVERIFIED)
    int32_t  m_unk238;                         // +0x238  second half of the same param
    bool     m_unk23C;                         // +0x23C  ctor 0 (role UNVERIFIED)
    // Engine-written RTPC shadows: the ctor registers these addresses with the audio
    // system (vf+0x40) for "volume_master"/"volume_music"/"volume_sfx".
    float    m_rtpcVolumeMaster;               // +0x240
    float    m_rtpcVolumeMusic;                // +0x244
    float    m_rtpcVolumeSfx;                  // +0x248
    uint8_t  _pad24C[4];                       // +0x24C
    // Proxy cvars created by the ctor (sub_180E3E564, VF_DUMPTODISK, change-callback
    // sub_180C09DFC). NOTE: s_MasterVolume's help string says "Simulates volume_voice
    // rtpc." -- Warhorse copy/paste bug, it drives volume_master.
    ::ICVar* m_pCVarMasterVolume;              // +0x250  "s_MasterVolume"
    ::ICVar* m_pCVarMusicVolume;               // +0x258  "s_MusicVolume"
    ::ICVar* m_pCVarSoundVolume;               // +0x260  "s_SoundVolume"

    std::vector<S_Resolution> m_resolutions;   // +0x268  display modes >= 1280x720 (RebuildResolutionList)
    int32_t  m_resolutionIndex;                // +0x280  index of r_Width x r_Height in m_resolutions (FindResolutionIndex)
    int32_t  m_aspectRatioIndex;               // +0x284  nearest-standard-aspect index (sub_18053DFD0 lookup; name INFERRED)
    std::vector<int32_t> m_refreshRates;       // +0x288  (RebuildRefreshRateList; fallback {30, 60})
    int32_t  m_refreshRateIndex;               // +0x2A0  ctor: count-1 (highest)
    int32_t  m_unk2A4;                         // +0x2A4  ctor -1 (role UNVERIFIED)
    int32_t  m_unk2A8;                         // +0x2A8  ctor -1 (role UNVERIFIED)
    uint8_t  _pad2AC[0xC];                     // +0x2AC  ctor-unwritten
    wh::C_ReadinessObserver m_specsChangeObserver;   // +0x2B8  "SpecsChange" readiness event
    // Busy-protection window: -1.0f idle; flash "SetBusyProtection"(true) wrapper
    // 0x181F8B2D0 stamps the current time + enables the "no_input" input filter;
    // Update auto-runs the off-wrapper 0x181F8C270 one second later.
    float    m_busyProtectionTime;             // +0x328  ctor -1.0f
    uint8_t  _pad32C[4];                       // +0x32C
    C_UIMenu* m_pMenu;                         // +0x330  owner backptr (ctor a2); receiver of PreparePage/ShowPage
    C_UIKeybinds      m_keybinds;              // +0x338  EMBEDDED (ctor sub_1806003C8); Init/Update cascaded
    C_UIPhotomodeMenu m_photomode;             // +0x398  EMBEDDED (ctor sub_181F89F00); Init cascaded (no Update)
};
static_assert(sizeof(C_UISettings) == 0x490, "C_UISettings must be 0x490 (C_UIMenu +0x1D8..+0x668)");
static_assert(offsetof(C_UISettings, m_gameSettingsSnapshot) == 0x68, "snapshots from 0x68 (sub_180F69FC0)");
static_assert(offsetof(C_UISettings, m_pCVarSubtitlesEnabled) == 0x128, "cvar cache from 0x128 (ctor)");
static_assert(offsetof(C_UISettings, m_rtpcVolumeMaster) == 0x240, "rtpc shadows at 0x240 (ctor audio registration)");
static_assert(offsetof(C_UISettings, m_resolutions) == 0x268, "resolution vector at 0x268 (0x180F6D9CC)");
static_assert(offsetof(C_UISettings, m_specsChangeObserver) == 0x2B8, "SpecsChange observer at 0x2B8");
static_assert(offsetof(C_UISettings, m_busyProtectionTime) == 0x328, "busy-protection stamp at 0x328");
static_assert(offsetof(C_UISettings, m_pMenu) == 0x330, "menu backptr at 0x330 (builders read a1[102])");
static_assert(offsetof(C_UISettings, m_keybinds) == 0x338, "keybinds at 0x338");
static_assert(offsetof(C_UISettings, m_photomode) == 0x398, "photomode at 0x398");

}  // namespace wh::guimodule
