// MCM_API.h — public API of the MCM (Mod Configuration Menu) KCSE plugin.
// Consuming plugins vendor this header; there is no import lib.
//
// Lua mods use the scriptbind counterpart instead (global published before
// main.lua / mod init scripts run; dot-call, not colon-call):
//     MCM.RegisterBuildSettingsListener(function()
//         MCM.AddMod("my_mod", "My Mod")
//         MCM.AddToggle("my_mod", "enable", "Enable Mod", "tooltip", 1)
//         MCM.AddSlider("my_mod", "speed", "Speed", "tooltip", 0, 10, 0.5, 1, "x")
//         MCM.SetValue("my_mod", "speed", GetLiveSpeed())  -- optional: live != default
//     end)
//     MCM.RegisterValueChangeListener("my_mod", function(settingId, value) ... end)

#pragma once

#include <cstdint>

#include "KCSE/KCSEAPI.h"

namespace MCM {

    // Broadcast by the MCM plugin (sender "MCM") on every setting mutation —
    // manual edits and X resets alike; sliders fire live during drags.
    // MCM keeps values in memory only: consuming plugins own persistence.
    constexpr uint32_t kMessage_ValueChanged = 0x4D434D01;   // 'MCM',1

    // msg->data for kMessage_ValueChanged. The pointers are only guaranteed
    // for the duration of the callback — copy what you keep.
    struct ValueChanged {
        const char* modId;      // <MCMConfig modid=...>
        const char* settingId;  // <Toggle|Dropdown|Slider id=...>
        double      value;      // toggle: 0/1, dropdown: option index, slider: value
    };

    // Broadcast by the MCM plugin (sender "MCM") every time the settings
    // list is rebuilt (each menu open). msg->data = ISettingsBuilder*, valid
    // ONLY for the duration of the callback: call Add* on it to contribute
    // settings programmatically (in addition to / instead of a Libs/UI/MCM
    // XML config). The list is rebuilt from scratch each time, so add your
    // full set on every broadcast; current session values are re-applied by
    // id afterwards.
    constexpr uint32_t kMessage_BuildSettings = 0x4D434D02;  // 'MCM',2

    struct ISettingsBuilder {
        virtual void AddMod(const char* modId, const char* displayName) = 0;
        virtual void AddCategory(const char* modId, const char* label,
                                 const char* tooltip) = 0;
        virtual void AddToggle(const char* modId, const char* id, const char* label,
                               const char* tooltip, bool defaultOn) = 0;
        virtual void AddDropdown(const char* modId, const char* id, const char* label,
                                 const char* tooltip, const char* options /* "A|B|C" */,
                                 int defaultIndex) = 0;
        virtual void AddSlider(const char* modId, const char* id, const char* label,
                               const char* tooltip, double mn, double mx, double step,
                               double defaultValue, const char* suffix) = 0;

        virtual void SetValue(const char* modId, const char* id, double value) = 0;
    };

    // Example of how a plugin would provide and consume its MCM settings:
    //     KCSE_PLUGIN_LOAD(kcse) {
    //         MCM::ListenForMessages([](KCSE::Message* msg) {
    //             if (msg->type == MCM::kMessage_BuildSettings) {
    //                 auto* b = static_cast<MCM::ISettingsBuilder*>(msg->data);
    //                 b->AddMod("my_mod", "My Mod");
    //                 b->AddToggle("my_mod", "enable", "Enable Mod", "tip", true);
    //                 b->SetValue("my_mod", "enable", g_liveEnabledFlag);
    //             } else if (msg->type == MCM::kMessage_ValueChanged) {
    //                 auto* v = static_cast<const MCM::ValueChanged*>(msg->data);
    //                 if (strcmp(v->modId, "my_mod") == 0)
    //                     ApplySetting(v->settingId, v->value);
    //             }
    //         });
    //         return true;
    //     }
    inline void ListenForMessages(KCSE::IMessagingInterface::EventCallback cb) {
        KCSE::GetMessagingInterface()->RegisterListener("MCM", cb);
    }
}  // namespace MCM
