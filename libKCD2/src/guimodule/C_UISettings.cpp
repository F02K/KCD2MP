#include "guimodule/C_UISettings.h"
#include "Offsets/Offsets.h"

// C_UISettings forwarders -- KCD2 WHGame.dll 1.5.6 RVAs (verified in kd7u).
//
// The page builders run against `this` (widget adds through the C_UIMenuBase tier)
// and against m_pMenu (+0x330) for PreparePage/ShowPage. Each ends with the standard
// Reset(3)/Apply(2)/Back(4) footer registrations. Widget-by-widget maps:
// analysis/ui_survey/menu_page_builders.md §2.

namespace wh::guimodule {

void C_UISettings::BuildGameSettingsPage()
{
    // sub_180F6AD04: page 11 -- LangVoice/LangSubtitles (localization-mgr populated),
    // EnableTutorials, AutomaticRoadMagnetism, EnableAdaptiveHud, CursorVisible,
    // EnableTelemetry, subtitle group (30/31/90/88/89); syncs m_gameSettingsSnapshot.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(52) };
    fn(this);
}

void C_UISettings::BuildGraphicSettingsPage()
{
    // sub_180F6C288: page 12 -- Resolution (m_resolutions), WindowMode, Quality,
    // ShowFPS, VSync, Gamma, FOV, upscaling group (84..87), camera-effects group
    // (56/57), HDR group (58/59); syncs m_graphicsSnapshot.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(55) };
    fn(this);
}

void C_UISettings::BuildAdvancedGraphicsPage()
{
    // sub_180F6B40C: page 13 -- Antialiasing ("SMAA *") + the 11 *Detail categories
    // (45..55, tiers 1..5); syncs m_advGraphicsSnapshot.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(53) };
    fn(this);
}

void C_UISettings::BuildSoundSettingsPage()
{
    // sub_180F6A0E0: page 14 -- value sliders 60/61/62 (min = wh_snd_volctrl_silence
    // - epsilon), pushes current volumes, fires the "audio_setup_audio_settings"
    // audio state.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(49) };
    fn(this);
}

void C_UISettings::BuildControlsPage()
{
    // sub_180F6A51C: page 15 -- "SetControlsContainers"(inputMode) + mouse group
    // (71/72), controller group (78..83, 75), PS5 group (74/76); syncs
    // m_controlsSnapshot.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(51) };
    fn(this);
}

void C_UISettings::BuildKeybindsPage()
{
    // sub_180F68614: page 16 -- one "KeybindsSetSuperaction" row per
    // wh::playermodule::S_KeybindsSuperaction (keybind mgr at env+0x128 -> +0x40),
    // populated through m_keybinds.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(41) };
    fn(this);
}

void C_UISettings::ShowRootPhotomodePage()
{
    // sub_181F8DE50: m_pMenu->PreparePage(RootPhotomode) -> flash "ShowRootMenu" ->
    // m_photomode.BuildCvarWidgets() -> m_pMenu->ShowPage().
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(77) };
    fn(this);
}

void C_UISettings::SnapshotCurrentSettings()
{
    // sub_180F69FC0: fills all five snapshot blocks (+0x68..+0x124) from the live
    // cvars/volumes -- the Apply/Reset comparison baseline.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(47) };
    fn(this);
}

float* C_UISettings::GetVolumes(float (&out)[3])
{
    // sub_180F6A08C: {master, music, sound} via s_*Volume->GetFVal; returns &out.
    using Fn = float*(__fastcall*)(C_UISettings*, float*);
    static REL::Relocation<Fn> fn{ REL::ID(48) };
    return fn(this, out);
}

void C_UISettings::PushVolumesToFlash(const float (&vols)[3])
{
    // sub_180F69B5C: SetValue(MasterVolume, vols[0]) / (MusicVolume, vols[1]) /
    // (SoundVolume, vols[2]).
    using Fn = void(__fastcall*)(C_UISettings*, const float*);
    static REL::Relocation<Fn> fn{ REL::ID(45) };
    fn(this, vols);
}

int C_UISettings::FindResolutionIndex(uint32_t width, uint32_t height)
{
    // sub_180F6D87C: exact {w, h} match in m_resolutions, else the entry minimizing
    // |dw| + |dh|.
    using Fn = int(__fastcall*)(C_UISettings*, uint32_t, uint32_t);
    static REL::Relocation<Fn> fn{ REL::ID(56) };
    return fn(this, width, height);
}

void C_UISettings::RebuildResolutionList()
{
    // sub_180F6D9CC: clears + re-enumerates display modes (renderer vf +0xE0),
    // filters >= 1280x720, dedups, falls back to the largest mode, sorts, and updates
    // m_aspectRatioIndex. Run on system event 4122 (display changed) and by the ctor.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(57) };
    fn(this);
}

void C_UISettings::RebuildRefreshRateList()
{
    // sub_181F8E080: clears + re-enumerates refresh rates (display-info vf +0x180);
    // pushes {30, 60} when the enumeration yields nothing.
    using Fn = void(__fastcall*)(C_UISettings*);
    static REL::Relocation<Fn> fn{ REL::ID(78) };
    fn(this);
}

}  // namespace wh::guimodule
