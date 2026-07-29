# `libKCD2`
[![C++17](https://img.shields.io/static/v1?label=standard&message=C%2B%2B17&color=blue&logo=c%2B%2B&&logoColor=white&style=flat)](https://en.cppreference.com/w/cpp/compiler_support)
[![Platform](https://img.shields.io/static/v1?label=platform&message=windows&color=dimgray&style=flat)](#)

Reverse-engineered headers and runtime library for **Kingdom Come: Deliverance 2** (CryEngine 3 + Warhorse modules). Includes **KCSE** (Kingdom Come Script Extender) — an SKSE-style native plugin framework.

> This is a hobby project. Not every RE'd member is guaranteed correct — verify before use. Contributions welcome.

## Build Dependencies

- [spdlog](https://github.com/gabime/spdlog)
- [Visual Studio 2022+](https://visualstudio.microsoft.com/)
  - Desktop development with C++
- [CMake 3.15+](https://cmake.org/)
- [vcpkg](https://github.com/microsoft/vcpkg)

## End User Dependencies

- [Kingdom Come: Deliverance 2](https://store.steampowered.com/app/1771300/Kingdom_Come_Deliverance_II/)

## Notes

- libKCD2 targets game version **1.5.6**. CryEngine's interfuscator shuffles vtable order in release builds — SDK headers cannot be used for direct virtual calls across versions.

## Developing a KCSE Plugin

1. Create your plugin repo under `Projects/` (or add it as a submodule):
   ```
   Projects/MyPlugin/
   ├── .buildenv/
   │   └── CMakeLists.txt
   └── src/
       └── plugin.cpp
   ```

2. Minimal `CMakeLists.txt`:
   ```cmake
   project(MyPlugin LANGUAGES CXX)

   file(GLOB_RECURSE PLUGIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../src/*.cpp")
   add_library(MyPlugin SHARED ${PLUGIN_SOURCES})

   target_precompile_headers(MyPlugin PRIVATE "${RE_BUILDENV}/PCH.h")
   target_compile_definitions(MyPlugin PRIVATE _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)
   target_compile_options(MyPlugin PRIVATE /utf-8 /W4 /wd4819 /wd4100)
   target_include_directories(MyPlugin PRIVATE
       ${RE_ROOT}/include ${RE_ROOT}/include/CryEngine/CryCommon ${CMAKE_CURRENT_SOURCE_DIR}/../src)
   target_link_libraries(MyPlugin PRIVATE kcd_re spdlog::spdlog)
   ```

3. Minimal `plugin.cpp`:
   ```cpp
   #include "KCSE/KCSEAPI.h"

   KCSE_PLUGIN_INFO("MyPlugin", "Author", 1);

   KCSE_PLUGIN_LOAD(kcse)
   {
       return true;
   }
   ```

Any subproject with a `.buildenv/CMakeLists.txt` is auto-discovered by the root build.

## Acknowledgements

Inspired by [SKSE](https://skse.silverlock.org/) and [CommonLibSSE](https://github.com/powerof3/CommonLibSSE).

## License

[GPLv3](LICENSE)
