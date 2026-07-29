#pragma once

#include <cstring>

#include "MCM_API.h"
#include "crysystem/SSystemGlobalEnvironment.h"
#include "Offsets/vtables/ICVar.h"
#include "persist.h"

// Autobrew's MCM (Mod Configuration Menu) integration.  Soft dependency on MCM.dll -- if it
// isn't installed, nothing ever broadcasts sender="MCM" messages, so HandleMcmMessage
// (registered via MCM::ListenForMessages in plugin.cpp) never fires and these defaults stand.
// Every knob is ALSO a console CVar (kcse_autobrew_*, plugin.cpp binds this storage directly);
// int not bool because IConsole::RegisterCVarInt wants int storage.
//
// PERSISTENCE (persist.h) is MCM-ONLY by design: every menu edit is recorded into
// Mods/Autobrew/mod.cfg, which the engine replays next launch (values held by the console
// until our cvars register).  Console edits apply for the session but are NOT recorded.
// BuildSettings pushes the live values back with SetValue so the menu shows the persisted
// state, not the defaults.

// Balanced-by-default (user decision 2026-07-22): perk wall, manual-brew wall and Realistic
// mistakes all start ON -- opting OUT is the cheat, not opting in.
inline int   g_enableAutobrew = 1;   // main switch, START gate only: prompt hidden while off
inline int   g_enableLooping  = 1;   // off = one brew per press (vanilla result popup returns)
inline int   g_requirePerk    = 1;   // wall Auto Brew behind the Routine perk (soul ability 200)
inline int   g_requireBrewed  = 1;   // wall each recipe behind one manual brew (game's brewed flag)
inline int   g_mistakeMode    = 1;   // 0 Off / 1 Realistic / 2 Hardcore (core/Mistakes.h model)
inline float g_mistakeIntensity = 1.0f;   // multiplies boil jitter + slip chances (0.5..2.0)

inline constexpr const char* kMcmModId          = "autobrew";
inline constexpr const char* kMcmModName        = "Autobrew";
inline constexpr const char* kSettingEnable     = "enable";
inline constexpr const char* kSettingLooping    = "enable_looping";
inline constexpr const char* kSettingRequirePerk = "require_perk";
inline constexpr const char* kSettingRequireBrewed = "require_brewed";
inline constexpr const char* kSettingMistakes   = "mistakes";
inline constexpr const char* kSettingMistakeIntensity = "mistake_intensity";

// setting <-> cvar <-> storage bindings; exactly one store pointer is set per row.  Drives
// the ValueChanged write-through AND the BuildSettings SetValue reflection.
struct S_McmBinding {
    const char* settingId;
    const char* cvarName;
    int*        intStore;
    float*      floatStore;
};
inline constexpr S_McmBinding kMcmBindings[] = {
    { kSettingEnable,           "kcse_autobrew_enable",            &g_enableAutobrew,  nullptr },
    { kSettingLooping,          "kcse_autobrew_enable_looping",    &g_enableLooping,   nullptr },
    { kSettingRequirePerk,      "kcse_autobrew_require_perk",      &g_requirePerk,     nullptr },
    { kSettingRequireBrewed,    "kcse_autobrew_require_brewed",    &g_requireBrewed,   nullptr },
    { kSettingMistakes,         "kcse_autobrew_mistakes",          &g_mistakeMode,     nullptr },
    { kSettingMistakeIntensity, "kcse_autobrew_mistake_intensity", nullptr, &g_mistakeIntensity },
};

inline void HandleMcmMessage(KCSE::Message* msg) {
    if (msg->type == MCM::kMessage_BuildSettings) {
        auto* b = static_cast<MCM::ISettingsBuilder*>(msg->data);
        b->AddMod(kMcmModId, kMcmModName);
        b->AddCategory(kMcmModId, "Main", "");
        b->AddToggle(kMcmModId, kSettingEnable, "Enable Autobrew",
            "Master switch: show the Auto Brew prompt in the recipe book and drive the "
            "brewing automation.  Turning it off cancels a running brew.", true);
        b->AddToggle(kMcmModId, kSettingLooping, "Enable Looping",
            "Keep brewing the same recipe until the ingredients run out, with the results "
            "shown as toasts.  Off = one brew per press with the normal result popup.", true);
        b->AddCategory(kMcmModId, "Balancing", "");
        b->AddToggle(kMcmModId, kSettingRequirePerk, "Require the Routine Perk",
            "Auto Brew must first be unlocked by buying the Routine perk in the Alchemy "
            "perk tree (level 12).  Off = available from the start.", true);
        b->AddToggle(kMcmModId, kSettingRequireBrewed, "Require Manual Brew Once",
            "Auto Brew only appears on recipes whose potion Henry has brewed by hand at "
            "least once (the game's own brewed-before flag, so potions brewed before "
            "installing this mod count).", true);
        b->AddDropdown(kMcmModId, kSettingMistakes, "Brewing Mistakes",
            "Autobrew makes the mistakes Henry would make at his alchemy skill -- mis-timed "
            "boils, skipped grinds, wrong amounts -- graded by the game as usual; Hardcore "
            "adds temperature and finishing blunders and raises the rates.",
            "Off|Realistic|Hardcore", 1);
        b->AddSlider(kMcmModId, kSettingMistakeIntensity, "Mistake Intensity",
            "Scales how often and how badly mistakes happen (boil-timing jitter and slip "
            "chances alike).  1x is tuned to the game's own skill tolerance.",
            0.5, 2.0, 0.05, 1.0, "x");
        // Reflect the LIVE values (persisted edits, console edits) -- without this the menu
        // would show the Add* defaults after a restart even though the settings applied.
        for (const auto& bind : kMcmBindings)
            b->SetValue(kMcmModId, bind.settingId,
                        bind.floatStore ? static_cast<double>(*bind.floatStore)
                                        : static_cast<double>(*bind.intStore));
    } else if (msg->type == MCM::kMessage_ValueChanged) {
        auto* v = static_cast<const MCM::ValueChanged*>(msg->data);
        if (std::strcmp(v->modId, kMcmModId) != 0) return;
        for (const auto& bind : kMcmBindings) {
            if (std::strcmp(v->settingId, bind.settingId) != 0)
                continue;
            // Write THROUGH the cvar (its bound storage IS the global) so a console
            // inspection of kcse_autobrew_* shows the menu value too.
            auto* env = SSystemGlobalEnvironment::GetInstance();
            ICVar* cvar = env && env->pConsole ? env->pConsole->GetCVar(bind.cvarName) : nullptr;
            if (cvar) {
                if (bind.floatStore)
                    cvar->SetFloat(static_cast<float>(v->value));
                else
                    cvar->SetInt(static_cast<int>(v->value));
            } else {   // cvar not registered (never the case for in-game menu edits)
                if (bind.floatStore)
                    *bind.floatStore = static_cast<float>(v->value);
                else
                    *bind.intStore = static_cast<int>(v->value);
            }
            Autobrew::PersistSetting(bind.cvarName, v->value, bind.floatStore != nullptr);
            break;
        }
    }
}
