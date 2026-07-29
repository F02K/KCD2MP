#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::E_HudElements -- KCD2 WHGame.dll 1.5.6 (kd7u).  28 HUD widget ids.
// -----------------------------------------------
// The per-widget id space of the in-game "hud" flash movie. FULLY VERIFIED from the two
// C_UIHudMask converters: name->id sub_180556410 (strcmp chain) and id->name
// sub_180555AC0 (switch, "<invalid enum value>" default). Enumerated 0..27 by the
// per-element iterator sub_180C3BF10 (calls the visibility functor for ids 0..27) and
// bounds-checked to 28 by the bitset writer sub_180556744 ("invalid bitset position").
// Mangled uses appear as W4Type@E_HudElements@framework@wh (std::function TDs 0x184CCF060..),
// i.e. the enum is nested as E_HudElements::Type.
//
// [FUNDAMENTAL vs KCD1] KCD1's HUD element set was a 24-entry BIT MASK on C_UIEHud
// (RatioStrips = bit 20, mask 0x100000); KCD2 is a plain 28-value INDEX enum owned by
// framework (RatioStrips = 22) -- see guimodule/C_UIHudMask.h for the visibility machinery.

namespace wh::framework {

struct E_HudElements {
    enum Type : uint8_t {
        Compass          = 0,
        Stats            = 1,
        QAMWeapon        = 2,
        QAMFood          = 3,
        Subtitles        = 4,
        InfoText         = 5,
        GameLog          = 6,
        Hints            = 7,
        DialogLeft       = 8,
        DialogRight      = 9,
        Cursor           = 10,
        Crime            = 11,
        Wanted           = 12,
        PopUpBackground  = 13,
        TutorialMessage  = 14,
        FancyEvent       = 15,
        SkillCheck       = 16,
        ItemTransfer     = 17,
        Buffs            = 18,
        CommonEvent      = 19,
        DiceCursor       = 20,
        Trespassing      = 21,
        RatioStrips      = 22,   // the dialogue/cutscene letterbox strips (KCD1: bit 20)
        ShootingContest  = 23,
        Bubbles          = 24,
        TutorialInDialog = 25,
        DiceContainer    = 26,
        Vignette         = 27,

        Count            = 28,
    };
};

}  // namespace wh::framework
