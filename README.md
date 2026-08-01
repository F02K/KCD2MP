# KCD2MP

KCD2MP is a work-in-progress multiplayer framework for Kingdom Come: Deliverance II. The repository currently contains the breaking Version 0.4 protocol, Direct-IP networking, persistent dedicated server, and a KCSE-owned client foundation.

The project is forked from [KCD2ModLoader](https://github.com/xiaoxiao921/KCD2ModLoader) and uses
[ReturnOfModdingBase](https://github.com/xiaoxiao921/ReturnOfModdingBase) as its modding foundation.
The repository includes the project fork
[F02K/libKCD2](https://github.com/F02K/libKCD2) as a pinned Git submodule,
including its nested KCSE runtime, and
[F02K/Address-Library-For-KCSE](https://github.com/F02K/Address-Library-For-KCSE)
as the pinned source of KCSE's versioned address tables.
KCD2MP keeps the two loaders separate: `d3d12.dll` hosts the ImGui/modloader
frontend and `dinput8.dll` hosts the multiplayer client as a KCSE plugin.

> Version 0.4 uses the native libKCD2/KCSE multiplayer runtime. Every join runs
> an active engine probe before publishing capabilities: transform write/read,
> Actor/Soul/Human/Inventory readiness, item create/equip/unequip/delete, and
> Entity cleanup. Probe failures are reported as concrete client errors. The
> removed Game-Lua and signature paths are never used as fallbacks. See the
> [libKCD2/KCSE migration audit](docs/libkcd2-kcse-migration.md).

## Build tool

Requirements:

- Windows 10 or Windows 11
- Python 3.9 or newer
- CMake
- Visual Studio with the MSVC x64 C++ workload

After cloning, initialize the pinned vendor repositories with
`powershell -ExecutionPolicy Bypass -File tools/init_vendor.ps1`. See
[libKCD2 vendor integration](docs/libkcd2-vendor.md) for the update workflow.

Run `build.bat` from the repository root. On first launch it creates an isolated `.venv-build`
environment, installs the pinned Textual dependency, and opens the build TUI.

The TUI supports:

- Debug builds
- Optimized Release builds with debug symbols
- Automatic Kingdom Come: Deliverance II discovery through Steam
- A persistent game-directory override
- Native signature auditing with live output
- An explicit, fast-forward-only Address Library update with coverage validation
- A pinned vcpkg bootstrap for Protobuf, GameNetworkingSockets 1.5.1, Boost.Container, and spdlog
- Building and testing `d3d12.dll`, KCSE's `dinput8.dll`, `KCD2MPKCSEClient.dll`,
  `KCD2MPServer.exe`, protocol, server core, and networking
- Deploying both loaders, the KCSE multiplayer-client plugin, and every pinned
  Steam/GOG/Epic Address Library table

Debug, RelWithDebInfo, and Release all build the complete native KCSE
multiplayer client. Runtime engine objects remain isolated behind the copied
v4 client ABI.

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
`profile claim <player_id>`, `dummy spawn [name]`, `dummy remove <player_id>`,
`entities <disable|enable|status>`, `stop`, and `help`. Dummy players use the server's
default avatar, spawn two metres beside the first live player (or the configured world spawn),
occupy a normal player slot, and are not persisted.
`disable_non_player_entities = true` applies the disabled state to AI-controlled world entities
on every accepted client and late joiner. UI/Flash helpers, cameras, particles, equipment, and
other non-AI entities remain active. Passwords, identity tokens, and resume tokens
are never written to the log. Recovery claim codes are printed only when explicitly requested.

## Client

Load a save normally through KCD2, then open **Multiplayer → Open
Multiplayer**. The ImGui frontend calls the exact v4 C ABI exported by
`KCD2MPKCSEClient.dll`; it neither owns the network client nor receives engine
pointers.

The KCSE client resolves the local player with CryAction/GameContext and samples
the verified world-transform pointer from KCSE's `PostUpdate` task. Networking
uses bounded queues and never accesses game objects. DataLoaded, LoadGame,
SaveGame and NewGame events create a new runtime epoch and invalidate native
state.

Before a client can enter a world, the native runtime performs the complete
active probe and publishes its capability mask. The implementation uses the
audited `IEntity::SetWorldTM`, XGen/Actor/Soul/Human/Inventory lifecycle,
`IEntitySystemSink` order, physics proxy, equipment transactions, and native
world unload. There is no signature, Entity-name, Game-Lua, or console fallback.
See the [migration audit](docs/libkcd2-kcse-migration.md) for the exact
capability matrix.

Protocol v4 validates the game build, Address Library identity, KCSE version,
runtime capability mask, content fingerprint, and persistent identity. The
authoritative profile contains money, all 10 stats, all 35 skills and progress,
inventory instances, equipment slots, and avatar state. Capture and apply use
the same native reconciler, including rollback and rollback-failure unload.

## Supported game build and diagnostics

KCD2MP currently enables hooks only for Steam build `23914554` / WHGame build
`1308617_856` (game version 1.5). Other builds are reported as unsupported and continue without
KCD2MP hooks.

The diagnostic log console is always visible in Debug and Release; it is output-only. Startup
reports each major stage, the detected PE fingerprint, signature validation, and whether hooks
were enabled. A successful startup ends with:

```text
KCD2MP initialization completed - all required frontend signatures resolved - hooks enabled
```

Signatures can be checked without starting the game through the TUI's `Audit signatures` action.
The same native tool can be built and run from a terminal:

```powershell
cmake --build out\build\debug --config Debug --target KCD2MPSignatureAudit
out\build\debug\Debug\KCD2MPSignatureAudit.exe "C:\path\to\WHGame.dll"
```

Runtime and audit share one typed registry and the same Zydis-based call,
RIP-relative, VTable, and PE-range validation. These signatures belong to the
modloader/frontend; the KCSE multiplayer client does not consume them.

## Manual installation

Copy `d3d12_.dll` and KCSE's `dinput8.dll` to the directory containing
`KingdomCome.exe`; rename `d3d12_.dll` to `d3d12.dll`. Copy
`KCD2MPKCSEClient.dll` to
`<game-root>\mods\KCD2MP\KCSE\Plugins\`.
The default Steam destination is:

```text
KingdomComeDeliverance2\Bin\Win64MasterMasterSteamPGO
```

KCSE additionally requires the address-library file matching the installed distribution and
game build under `<game-root>\KCSE\addresslib\`. CMake bundles all tables from
`vendor\Address-Library-For-KCSE\kcd2_address_library`; the build tool deploys them together and
refuses to deploy `dinput8.dll` without them. KCSE intentionally fails closed when it cannot
resolve its versioned native addresses.

To uninstall KCD2MP and its KCSE integration, remove or rename `d3d12.dll`, `dinput8.dll`, and
`mods\KCD2MP\KCSE\Plugins\KCD2MPKCSEClient.dll`.

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
