#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_ButtonId -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : uint8.
// -----------------------------------------------
// Nested-Type enum wrapper (rttr property mangling `W4Type@E_ButtonId@guimodule@wh@@`).
// Identity key of C_UIMenuButton (byte @element+0x08; C_MenuButtonDatabase's
// DefaultKeyExtractor keys on it; runtime sorted-vector DB @0x1853266E0, 0x78 rows).
//
// ENUMERATORS VERIFIED: recovered 1:1 from the binary's own exhaustive id->name switch
// sub_180558EB8 (0..93; anything else returns "<invalid enum value>"). The returned
// names are the flash-protocol button ids AND the menu_buttons.xml Id strings (anchor
// cross-check: 60/61/62 = MasterVolume/MusicVolume/SoundVolume match the XML row order
// and the sound-page builder sub_180F6A0E0). Binary typos preserved verbatim
// (InvetrtMouse). Page usage evidence: analysis/ui_survey/menu_page_builders.md §4.

namespace wh::guimodule {

struct E_ButtonId {
    enum Type : uint8_t {
        Invalid                          = 0,
        Resume                           = 1,    // RootIngame/RootPause; magic flash behavior (close)
        Apply                            = 2,    // settings-page footer
        Reset                            = 3,    // settings-page footer
        Back                             = 4,    // every sub-page footer
        Continue                         = 5,    // RootMain
        TryAgain                         = 6,    // RootMain death variant
        NewGame                          = 7,
        NewGameDebug                     = 8,
        NewGameDebugTrosecko             = 9,
        NewGameDebugKlaster              = 10,
        SaveGame                         = 11,
        LoadGame                         = 12,
        PreviouslyKCD                    = 13,   // defined in DB, not used by any builder
        Settings                         = 14,
        HelpOverlays                     = 15,
        DLC                              = 16,
        Credits                          = 17,
        Exit                             = 18,
        SaveExit                         = 19,
        SaveReset                        = 20,   // debug-gated (dword_1855E2B70)
        GameSettings                     = 21,   // Settings hub (page 10)
        GraphicSettings                  = 22,
        AdvancedGraphics                 = 23,
        SoundSettings                    = 24,
        Controls                         = 25,
        Keybinds                         = 26,   // kb/mouse only (sub_180F69C84)
        PhotoMode                        = 27,   // ingame only
        LangVoice                        = 28,   // GameSettings (page 11) choices
        LangSubtitles                    = 29,
        EnableSubtitles                  = 30,
        EnableSubtitleBarks              = 31,
        EnableTutorials                  = 32,
        EnableTelemetry                  = 33,
        CursorVisible                    = 34,
        Resolution                       = 35,   // GraphicSettings (page 12) choices
        WindowMode                       = 36,
        Quality                          = 37,
        ShowFPS                          = 38,
        VSync                            = 39,
        RefreshRate                      = 40,   // defined, not seen in builders
        Gamma                            = 41,   // value slider
        FOV                              = 42,
        ConsoleMode                      = 43,   // defined, not seen in builders
        Antialiasing                     = 44,   // AdvanceGraphicSettings (page 13)
        ObjectDetail                     = 45,
        ParticlesDetail                  = 46,
        LightDetail                      = 47,
        GlobalIlluminationDetail         = 48,
        PostProcessingDetail             = 49,
        ShadingDetail                    = 50,
        ShadowsDetail                    = 51,
        TextureDetail                    = 52,
        VolumetricEffectsDetail          = 53,
        VegetationDetail                 = 54,
        CharacterDetail                  = 55,
        MotionBlur                       = 56,   // GraphicSettings camera-effects group
        NearDof                          = 57,
        HDROutput                        = 58,
        HDRUIBrightness                  = 59,   // value slider
        MasterVolume                     = 60,   // SoundSettings (page 14) sliders -- XML anchor
        MusicVolume                      = 61,
        SoundVolume                      = 62,
        TobiiMaster                      = 63,   // Tobii eye-tracking block: defined, not seen in builders
        TobiiExtendedViewEnable          = 64,
        TobiiExtendedViewSens            = 65,
        TobiiLockTargetGaze              = 66,
        TobiiInteractGaze                = 67,
        TobiiCleanUI                     = 68,
        NormalMode                       = 69,   // NewGameMode (pages 5/6)
        HardcoreMode                     = 70,
        MouseSensitivity                 = 71,   // Controls (page 15)
        InvetrtMouse                     = 72,   // (sic -- binary/XML typo)
        InvertMouseCombat                = 73,   // defined, not seen in builders
        ControllerAdaptiveTriggerEffect  = 74,   // PS5 group
        ControllerImmersiveTriggerEffect = 75,   // XBox group
        ControllerLightBarEffect         = 76,
        ControllerSpeakerEffect          = 77,   // defined, not seen in builders
        ControllerVibrations             = 78,
        ControllerDeadZone               = 79,
        ControllerVerticalInvert         = 80,
        ControllerHorizontalInvert       = 81,
        ControllerVerticalSensitivity    = 82,
        ControllerHorizontalSensitivity  = 83,
        UpscalingTechnology              = 84,   // GraphicSettings upscaling group
        UpscalingTechnologyDlssPreset    = 85,
        UpscalingMode                    = 86,
        UpscalingSharpness               = 87,
        EnableSubtitleSpeakerName        = 88,   // GameSettings subtitle group
        EnableSubtitleBg                 = 89,
        SubtitleSize                     = 90,
        AutomaticRoadMagnetism           = 91,
        EnableAdaptiveHud                = 92,
        InputDevice                      = 93,   // defined, not seen in builders
    };
};

}  // namespace wh::guimodule
