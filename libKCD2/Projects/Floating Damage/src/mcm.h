#pragma once

#include <cstring>

#include "MCM_API.h"

// Floating Damage's own MCM (Mod Configuration Menu) integration: the text-size/damage-scale
// sliders and the glue that keeps their live values in the g_* globals below. Soft dependency
// on MCM.dll -- if it isn't installed, nothing ever broadcasts sender="MCM" messages, so
// HandleMcmMessage (registered via MCM::ListenForMessages in plugin.cpp) simply never fires
// and these defaults stand.

inline float g_minTextSizePx  = 32.0f;
inline float g_maxTextSizePx  = 48.0f;
inline float g_minScaleDamage = 40.0f;   // |amount| at/below this reads at g_minTextSizePx
inline float g_maxScaleDamage = 350.0f;  // |amount| at/above this reads at g_maxTextSizePx
inline bool  g_onlyShowPlayerAttacks = false;  // hide popups for hits the player didn't deal (taken, or NPC-vs-NPC)
inline float g_decayStartDist   = 8.0f;  // meters; popups farther than this shrink inverse-linearly (0 = decay off). Player-DEALT hits never decay.
inline float g_minDistanceScale = 0.4f;  // decay floor as a fraction of the damage-scaled size (slider is in %)

inline constexpr const char* kMcmModId           = "floating_damage";
inline constexpr const char* kMcmModName         = "Floating Damage";
inline constexpr const char* kSettingMinTextSize = "min_text_size";
inline constexpr const char* kSettingMaxTextSize = "max_text_size";
inline constexpr const char* kSettingMinScaleDmg = "min_scale_damage";
inline constexpr const char* kSettingMaxScaleDmg = "max_scale_damage";
inline constexpr const char* kSettingOnlyPlayerAtk = "only_player_attacks";
inline constexpr const char* kSettingDecayStartDist = "decay_start_distance";
inline constexpr const char* kSettingMinDistScale   = "min_distance_scale";

inline void HandleMcmMessage(KCSE::Message* msg) {
    if (msg->type == MCM::kMessage_BuildSettings) {
        auto* b = static_cast<MCM::ISettingsBuilder*>(msg->data);
        b->AddMod(kMcmModId, kMcmModName);
        b->AddSlider(kMcmModId, kSettingMinTextSize, "Min Text Size",
            "Popup text size floor, in pixels (for the smallest hits).",
            16.0, 64.0, 1.0, 32, "px");
        b->AddSlider(kMcmModId, kSettingMaxTextSize, "Max Text Size",
            "Popup text size ceiling, in pixels (for the biggest hits).",
            16.0, 96.0, 1.0, 48, "px");
        b->AddSlider(kMcmModId, kSettingMinScaleDmg, "Min Scale Damage",
            "Damage at/below which the popup stays at Min Text Size.",
            0.0, 200.0, 5.0, 40, "");
        b->AddSlider(kMcmModId, kSettingMaxScaleDmg, "Max Scale Damage",
            "Damage at/above which the popup reaches Max Text Size.",
            50.0, 1000.0, 10.0, 350, "");
        b->AddToggle(kMcmModId, kSettingOnlyPlayerAtk, "Only Player Attacks",
            "Only show damage numbers for hits the player deals -- hides damage the "
            "player takes and NPC-vs-NPC fights.", false);
        b->AddSlider(kMcmModId, kSettingDecayStartDist, "Decay Start Distance",
            "Popups farther from the player than this shrink with distance "
            "(half size at double the distance). Player-dealt hits never shrink. 0 = off.",
            0.0, 50.0, 1.0, 8, "m");
        b->AddSlider(kMcmModId, kSettingMinDistScale, "Min Distance Scale",
            "Smallest a distance-shrunk popup can get, as a percentage of its normal size.",
            10.0, 100.0, 5.0, 40, "%");
    } else if (msg->type == MCM::kMessage_ValueChanged) {
        auto* v = static_cast<const MCM::ValueChanged*>(msg->data);
        if (std::strcmp(v->modId, kMcmModId) != 0) return;
        const float value = static_cast<float>(v->value);
        if (std::strcmp(v->settingId, kSettingMinTextSize) == 0) g_minTextSizePx = value;
        else if (std::strcmp(v->settingId, kSettingMaxTextSize) == 0) g_maxTextSizePx = value;
        else if (std::strcmp(v->settingId, kSettingMinScaleDmg) == 0) g_minScaleDamage = value;
        else if (std::strcmp(v->settingId, kSettingMaxScaleDmg) == 0) g_maxScaleDamage = value;
        else if (std::strcmp(v->settingId, kSettingOnlyPlayerAtk) == 0) g_onlyShowPlayerAttacks = v->value != 0.0;
        else if (std::strcmp(v->settingId, kSettingDecayStartDist) == 0) g_decayStartDist = value;
        else if (std::strcmp(v->settingId, kSettingMinDistScale) == 0) g_minDistanceScale = value * 0.01f;
    }
}
