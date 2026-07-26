# KCD2MP

KCD2MP is a work-in-progress multiplayer framework for Kingdom Come: Deliverance II.

The project is forked from [KCD2ModLoader](https://github.com/xiaoxiao921/KCD2ModLoader) and uses
[ReturnOfModdingBase](https://github.com/xiaoxiao921/ReturnOfModdingBase) as its modding foundation.

> Multiplayer functionality is not available yet. The current codebase provides the loader and
> modding framework that the multiplayer implementation will build on.

## Build tool

Requirements:

- Windows 10 or Windows 11
- Python 3.9 or newer
- CMake
- Visual Studio with the MSVC x64 C++ workload

Run `build.bat` from the repository root. On first launch it creates an isolated `.venv-build`
environment, installs the pinned Textual dependency, and opens the build TUI.

The TUI supports:

- Debug builds
- Optimized Release builds with debug symbols
- Automatic Kingdom Come: Deliverance II discovery through Steam
- A persistent game-directory override
- Native signature auditing with live output
- Building and deploying `d3d12.dll` and `d3d12.pdb`

`Build & Deploy` runs the signature audit against the installed `WHGame.dll` before copying any
files. The deploy action does not start or stop the game. Close the game before deploying so
Windows does not lock the DLL.

## Supported game build and diagnostics

KCD2MP currently enables hooks only for Steam build `23914554` / WHGame build
`1308617_856` (game version 1.5). Other builds are reported as unsupported and continue without
KCD2MP hooks.

The diagnostic console is always visible in Debug and Release. Startup reports each major stage,
the detected PE fingerprint, signature validation, and whether hooks were enabled. A successful
startup ends with:

```text
KCD2MP initialization completed - 64/64 signatures resolved - hooks enabled
```

Signatures can be checked without starting the game through the TUI's `Audit signatures` action.
The same native tool can be built and run from a terminal:

```powershell
cmake --build out\build\debug --config Debug --target KCD2MPSignatureAudit
out\build\debug\Debug\KCD2MPSignatureAudit.exe "C:\path\to\WHGame.dll"
```

Runtime and audit share one typed 64-entry registry and the same Zydis-based call, RIP-relative,
VTable, and PE-range validation.

## Manual installation

Copy the built loader to the directory containing `KingdomCome.exe` and name it `d3d12.dll`.
The default Steam destination is:

```text
KingdomComeDeliverance2\Bin\Win64MasterMasterSteamPGO
```

To uninstall KCD2MP, remove or rename `d3d12.dll`.

## Existing modding features

- Lua plugin loading and hot reload
- Dear ImGui Lua bindings ([ImGui API](docs/lua/tables/ImGui.md),
  [GUI API](docs/lua/tables/rom.gui.md))
- FMOD modding
- Generic game-file modifications
- ASI mod loading
- XML merging
- Debug inspectors and trainer utilities

## Test mod

The example plugin is available in [`examples/plugins/KCD2MP-TestMod`](examples/plugins/KCD2MP-TestMod).
