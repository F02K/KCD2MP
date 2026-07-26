# KCD2MP

KCD2MP is a work-in-progress multiplayer framework for Kingdom Come: Deliverance II. The repository currently contains the Version 0.2 protocol, Direct-IP networking, persistent dedicated server, and in-DLL client foundation.

The project is forked from [KCD2ModLoader](https://github.com/xiaoxiao921/KCD2ModLoader) and uses
[ReturnOfModdingBase](https://github.com/xiaoxiao921/ReturnOfModdingBase) as its modding foundation.

> Version 0.2 is not release-ready yet. The signature-gated retail bootstrap can load an
> isolated server-selected level from the main menu without opening a single-player save.
> RPG/inventory application, remote-human avatars, and full in-game acceptance testing remain
> incomplete. See [Version 0.2 status](docs/multiplayer-0.2.md).

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
- A pinned vcpkg bootstrap for Protobuf and GameNetworkingSockets 1.5.1
- Building and testing `d3d12.dll`, `KCD2MPServer.exe`, protocol, server core, and networking
- Deploying `d3d12.dll` and `d3d12.pdb`

`Build & Deploy` runs the signature audit against the installed `WHGame.dll` before copying any
files. The deploy action does not start or stop the game. Close the game before deploying so
Windows does not lock the DLL.

## Dedicated server

Copy `server.toml.example` to `server.toml`, choose the canonical `level_id` for the sandbox,
and then start:

```powershell
KCD2MPServer.exe server.toml
```

Retail world loading currently accepts the numeric IDs from the supported game tables. The main
worlds are `2` (`trosecko`), `3` (`kutnohorsko`), and `4` (`klaster`).

The standalone Windows-x64 server does not load or require KCD2. It listens on UDP port `27020`
by default. Allow that UDP port through Windows Firewall and forward it on the router only when
hosting beyond the LAN.

The `world_directory` contains the persistent session manifest and player profiles. Server
console commands are `status`, `players`, `kick <player_id> [reason]`, `say <text>`,
`profile claim <player_id>`, `stop`, and `help`. Passwords, identity tokens, and resume tokens
are never written to the log. Recovery claim codes are printed only when explicitly requested.

## Client

Open **Multiplayer → Open Multiplayer** from the real game frontend without loading a save.
Enter a numeric `host:port`, display name, and optional password. Only address and display name
are persisted; password and recovery-code fields are cleared after each connect attempt.

Connect becomes available when the audited retail console, required CVars, level command, and
player-transform wrapper are present. After the server bootstrap arrives, the client locks native
save/load, freezes the playline, skips the retail new-game intro, loads the server-selected level,
waits for a stable player entity and matching level, applies the persisted server transform, and
only then reports the world ready. The native level-start sequence remains enabled so KCD2 can
finish dismissing its loading UI. No existing save is loaded, copied, modified, or uploaded as a
fallback.

The game console also provides:

```text
mp_connect <host:port> [name]
mp_disconnect
mp_status
mp_say <text>
```

Password-protected connections must be started from ImGui so the password is not retained in
console history. Protocol version, KCD2MP version, WHGame build, password, optional content hash,
persistent identity, session manifest, profile revision, and the eight-player limit are validated
during the two-stage handshake.

See [Version 0.2 architecture, persistence, safety gates, tests, and known limits](docs/multiplayer-0.2.md).

## Supported game build and diagnostics

KCD2MP currently enables hooks only for Steam build `23914554` / WHGame build
`1308617_856` (game version 1.5). Other builds are reported as unsupported and continue without
KCD2MP hooks.

The diagnostic console is always visible in Debug and Release. Startup reports each major stage,
the detected PE fingerprint, signature validation, and whether hooks were enabled. A successful
startup ends with:

```text
KCD2MP initialization completed - 65/65 signatures resolved - hooks enabled
```

Signatures can be checked without starting the game through the TUI's `Audit signatures` action.
The same native tool can be built and run from a terminal:

```powershell
cmake --build out\build\debug --config Debug --target KCD2MPSignatureAudit
out\build\debug\Debug\KCD2MPSignatureAudit.exe "C:\path\to\WHGame.dll"
```

Runtime and audit share one typed 65-entry registry and the same Zydis-based call, RIP-relative,
VTable, and PE-range validation. The supported retail image currently resolves 80/80 derived
targets, including the console world-loading entry point.

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
