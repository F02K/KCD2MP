#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_ConfirmationId -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : uint8.
// -----------------------------------------------
// Nested-Type enum wrapper (rttr property mangling `W4Type@E_ConfirmationId@guimodule@wh@@`).
// Identity key of C_UIMenuConfirmation (byte @element+0x08; C_MenuConfirmationDatabase's
// DefaultKeyExtractor keys on it).
//
// ENUMERATORS VERIFIED from two independent sources that agree on all 27 values:
// ToString switch sub_182BAAD2C (case -> literal) and the rttr enumeration registration
// sub_181AC2170 ("wh::guimodule::ConfirmationId", {name, len, value} triples).
// ControlsConflict (13) is the keybind-conflict blocker raised by the keybinds-page
// Apply/Back gates (sub_181F91630 / sub_181F90490) when C_Keybinds::HasConflicts()
// returns true -- see analysis/keybind_conflicts_re.md.

namespace wh::guimodule {

struct E_ConfirmationId {
    enum Type : uint8_t {
        Invalid                                  = 0,
        NewGameLostProgress                      = 1,
        ExitGameLostProgress                     = 2,
        SaveExitGame                             = 3,
        SaveResetGame                            = 4,
        ExitGame                                 = 5,
        LoadSavedGame                            = 6,
        DeleteSavedGame                          = 7,
        Reset                                    = 8,
        Apply                                    = 9,
        ApplyWarningUltraSettings                = 10,
        BackWarningUltraSettings                 = 11,
        Back                                     = 12,
        ControlsConflict                         = 13,   // keybind-conflict block dialog
        LanguageNotDownloaded                    = 14,
        LanguageCantDownloaded                   = 15,
        LoadFailed                               = 16,
        GameSaved                                = 17,
        GameSavedFailed                          = 18,
        OverwriteSave                            = 19,
        OverwritePlayline                        = 20,
        LoadOutdatedSave                         = 21,
        ApplyWarningUpscaling                    = 22,
        BackWarningUpscaling                     = 23,
        SaveManualGame                           = 24,
        LanguageNotDownloadedFaceAnimNeedRestart = 25,
        FaceAnimNeedRestart                      = 26,
    };
};

}  // namespace wh::guimodule
