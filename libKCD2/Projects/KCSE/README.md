# `KCSE`
[![C++17](https://img.shields.io/static/v1?label=standard&message=C%2B%2B17&color=blue&logo=c%2B%2B&&logoColor=white&style=flat)](https://en.cppreference.com/w/cpp/compiler_support)
[![Platform](https://img.shields.io/static/v1?label=platform&message=windows&color=dimgray&style=flat)](#)

Kingdom Come Script Extender — an SKSE-style native plugin framework for **Kingdom Come: Deliverance 2**. Ships as a dinput8.dll proxy, no exe patching required.

## Build Dependencies

- [libKCD2](https://github.com/JerryYOJ/libKCD2)
- [spdlog](https://github.com/gabime/spdlog)
- [Visual Studio 2022+](https://visualstudio.microsoft.com/)
  - Desktop development with C++
- [CMake 3.15+](https://cmake.org/)
- [vcpkg](https://github.com/microsoft/vcpkg)

## End User Dependencies

- [Kingdom Come: Deliverance 2](https://store.steampowered.com/app/1771300/Kingdom_Come_Deliverance_II/) (1.5.6)

## Installation

Copy `dinput8.dll` to `<game>/Bin/Win64MasterMasterSteamPGO/`.

## Plugin Development

```cpp
#include "KCSE/KCSEAPI.h"

KCSE_PLUGIN_INFO("MyPlugin", "Author", 1);

KCSE_PLUGIN_LOAD(kcse)
{
    kcse->GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) {
        if (msg->type == KCSE::IMessagingInterface::kMessage_DataLoaded) {
            // Game data loaded
        }
    });

    return true;
}
```

Plugins are DLLs placed in `mods/<modname>/KCSE/Plugins/` or `KCSE/Plugins/`.

## Notes

- KCSE auto-calls `KCSE::Init()` when using the `KCSE_PLUGIN_LOAD` macro.
- Available lifecycle messages: `PreDataLoaded` (before main.lua / mod init scripts — publish Lua globals here), `DataLoaded`, `LoadGame`, `SaveGame`, `NewGame`, `AllPluginsLoaded`.
- Per-frame tasks via `KCSE::GetTaskInterface()->AddTask(fn)`.
- Trampoline hooking via `KCSE::GetTrampoline()`.

## License

[GPLv3](LICENSE)
