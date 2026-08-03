# KCD2MP

Experimental multiplayer for Kingdom Come: Deliverance II.

> [!WARNING]
> KCD2MP **v0.0.9 is a prototype**, not a production-ready multiplayer mod.
> Expect breaking changes, incomplete world simulation, compatibility limits,
> and loss of multiplayer-world data while development continues. Use test
> saves and keep backups of anything important.

## Project status

| | |
| --- | --- |
| Current version | **0.0.9** |
| Development stage | Prototype / technical preview |
| Networking | Direct IP, dedicated authoritative server |
| Platform | Windows x64 |
| Supported game | Steam build `23914554`, game version 1.5 |
| Supported WHGame | `1308617_856` |

KCD2MP uses one project version across the client, server, build metadata, and
network handshake. During the prototype phase, clients and servers must run the
exact same KCD2MP version. There is no separate user-facing "protocol version".
See [CHANGELOG.md](CHANGELOG.md) for version history.

## What works in v0.0.9

- Direct-IP client/server connection with authentication and reconnect support
- Persistent server sessions and player profiles
- Remote-player spawning, movement, appearance, equipment, and weapon state
- Native inventory and equipment reconciliation with rollback
- Server-authoritative doors and loot containers
- Supported containers: regular chests, cart chests, stash corpses, bird nests,
  and destructible stashes
- Server-authoritative dropped items, pickup ownership, and restart persistence
- Shared time-of-day, time scale, and weather
- Independent human- and animal-NPC isolation controls
- Signature and Address Library validation before native hooks are enabled

NPC world synchronization, quests, dialogue state, combat AI, and complete
cooperative world progression are **not implemented yet**. Native NPCs can be
isolated, but they are not currently replicated as shared server entities.

The detailed implementation status and current limits are documented in
[docs/multiplayer.md](docs/multiplayer.md).

## Architecture

KCD2MP keeps its two runtime boundaries separate:

- `d3d12.dll` provides the mod-loader and ImGui frontend.
- `dinput8.dll` hosts KCSE.
- `KCD2MPKCSEClient.dll` owns the in-game multiplayer client and native game
  integration.
- `KCD2MPServer.exe` is a standalone dedicated server and does not load KCD2.

The project is based on
[KCD2ModLoader](https://github.com/xiaoxiao921/KCD2ModLoader) and
[ReturnOfModdingBase](https://github.com/xiaoxiao921/ReturnOfModdingBase).
It pins [F02K/libKCD2](https://github.com/F02K/libKCD2) and
[F02K/Address-Library-For-KCSE](https://github.com/F02K/Address-Library-For-KCSE)
as vendor dependencies.

Native engine access is capability-gated. A client join verifies the game
build, KCSE/libKCD2 runtime, Address Library identity, required native features,
content fingerprint, and KCD2MP version before entering the world. Runtime
objects never cross the frontend/client ABI boundary.

## Build and deploy

Requirements:

- Windows 10 or Windows 11
- Python 3.9 or newer
- CMake
- Visual Studio with the MSVC x64 C++ workload

Initialize the pinned vendor repositories after cloning:

```powershell
powershell -ExecutionPolicy Bypass -File tools/init_vendor.ps1
```

Run `build.bat` from the repository root. On first launch, the build tool
creates an isolated `.venv-build` environment and installs its pinned Python
dependencies.

The terminal UI can:

- build Debug or optimized Release artifacts with symbols;
- run native, protocol, server, networking, and deployment tests;
- discover the Steam installation or remember a manual game path;
- audit the installed `WHGame.dll` before deployment;
- validate and deploy the pinned Steam/GOG/Epic Address Library tables; and
- deploy both loaders and the KCSE client plugin.

Every successful build also creates a clean package tree under
`out/package/<debug|release>/`:

```text
client/   install-ready game tree and KCD2MP-Client-v0.0.9.zip
server/   dedicated server, configuration, data, symbols, and audit tool
tests/    test executables and their symbols only
SHA256SUMS.txt
```

The client ZIP starts with `KingdomComeDeliverance2/` and mirrors the same
relative paths used by `Build & Deploy`. It can therefore be extracted directly
into the Steam `steamapps/common` directory. See
[Build and release packaging](docs/build-packaging.md) for the exact layout and
standalone packaging command.

`Build & Deploy` never starts or stops the game. Close KCD2 before deployment so
Windows can replace the runtime DLLs.

## Dedicated server

Copy `server.toml.example` to `server.toml`, select the sandbox `level_id`, and
start the server:

```powershell
KCD2MPServer.exe server.toml
```

The common retail world IDs are:

- `2` — Trosky region (`trosecko`)
- `3` — Kuttenberg region (`kutnohorsko`)
- `4` — Monastery (`klaster`)

The server listens on UDP port `27020` by default. Allow and forward that port
only when hosting outside the LAN.

Persistent session data, player profiles, synchronized world objects, and
dropped items are stored below `world_directory`. Writes use temporary sibling
files followed by atomic replacement. Native save files are never uploaded to
or read by the dedicated server.

Available server commands include `status`, `players`, `kick`, `say`,
`profile claim`, `dummy spawn`, `dummy remove`, `entities`, `time`, `timescale`,
`weather`, `stop`, and `help`.

## Joining from the game

1. Start KCD2 and load a normal save through the game's UI.
2. Open **Multiplayer -> Open Multiplayer** in the KCD2MP frontend.
3. Enter the server address and connect.

The native client adopts the loaded world only when its level matches the
server. It then applies the multiplayer profile and sends `WorldReady`. Failed
native prerequisites stop the join with a concrete error instead of falling
back to unsafe signatures, generated Lua, or guessed entity names.

## Manual installation

For manual deployment:

1. Copy `d3d12_.dll` beside `KingdomCome.exe` and rename it to `d3d12.dll`.
2. Copy KCSE's `dinput8.dll` beside `KingdomCome.exe`.
3. Copy `KCD2MPKCSEClient.dll` to
   `<game-root>\mods\KCD2MP\KCSE\Plugins\`.
4. Copy the matching Address Library table to
   `<game-root>\KCSE\addresslib\`.

The default Steam binary directory is:

```text
KingdomComeDeliverance2\Bin\Win64MasterMasterSteamPGO
```

To uninstall the runtime, remove or rename `d3d12.dll`, `dinput8.dll`, and
`mods\KCD2MP\KCSE\Plugins\KCD2MPKCSEClient.dll`.

## Diagnostics and testing

Debug and Release builds expose an output-only diagnostic console. Successful
startup reports the detected PE fingerprint, signature validation, and enabled
hooks. The build tool can run the same signature audit without starting KCD2.

Automated coverage includes protocol validation, server lifecycle and
persistence, player profiles, identity storage, remote avatars, container and
door conflicts, dropped-item ownership, environment state, native capability
gates, deployment behavior, and Address Library coverage.

In-game multi-client soak tests, save-directory comparison, fault injection,
and full NPC/quest simulation remain manual or future work.

## Additional documentation

- [Multiplayer architecture and status](docs/multiplayer.md)
- [Version history](CHANGELOG.md)
- [libKCD2/KCSE migration audit](docs/libkcd2-kcse-migration.md)
- [Vendor integration](docs/libkcd2-vendor.md)
- [Build and release packaging](docs/build-packaging.md)

## Existing mod-loader features

The inherited modding layer also provides Lua plugin loading and hot reload,
Dear ImGui Lua bindings, FMOD integration, ASI loading, XML merging, and debug
inspection tools. The example plugin is in
[`examples/plugins/KCD2MP-TestMod`](examples/plugins/KCD2MP-TestMod).
