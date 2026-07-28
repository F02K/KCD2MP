# KCD2MP

KCD2MP is a work-in-progress multiplayer framework for Kingdom Come: Deliverance II. The repository currently contains the Version 0.3 protocol, Direct-IP networking, persistent dedicated server, and in-DLL client foundation.

The project is forked from [KCD2ModLoader](https://github.com/xiaoxiao921/KCD2ModLoader) and uses
[ReturnOfModdingBase](https://github.com/xiaoxiao921/ReturnOfModdingBase) as its modding foundation.

> Version 0.3 is not release-ready yet. The signature-gated retail bootstrap adopts a natively
> loaded save on the server-selected level. RPG/inventory application, the audited native
> remote-human backend, and full in-game acceptance testing remain incomplete. See
> [Version 0.3 status](docs/multiplayer-0.2.md).

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
`profile claim <player_id>`, `dummy spawn [name]`, `dummy remove <player_id>`,
`entities <disable|enable|status>`, `stop`, and `help`. Dummy players use the server's
default avatar, spawn two metres beside the first live player (or the configured world spawn),
occupy a normal player slot, and are not persisted.
`disable_non_player_entities = true` applies the disabled state to every accepted client and
late joiner. Passwords, identity tokens, and resume tokens
are never written to the log. Recovery claim codes are printed only when explicitly requested.

## Client

Load a save normally through KCD2, then open **Multiplayer → Open Multiplayer**. Enter a numeric
`host:port`, display name, and optional password. Only address and display name are persisted;
password and recovery-code fields are cleared after each connect attempt.

Connect becomes available when the audited retail console, required CVars, unload command, and
player-transform wrapper are present. The loaded save's live
`wh_sys_BaseLevelId` must match the server. After the bootstrap arrives, the client locks native
save/load and autosave, adopts the already running world, waits for a stable player entity, applies
the persisted server transform, and only then reports the world ready. KCD2MP does not read, copy,
modify, or upload the selected `.whs` file. Disconnect unloads the adopted world before restoring
temporary CVars. The direct-map bootstrap is retained only for developer diagnosis because both
post-load and pre-map native-start experiments fail to complete the retail loading-screen
transition. Once the server accepts the ready client, the mod GUI closes automatically and returns
mouse and keyboard input to the game unless the developer console is open.

The local **GUI -> Developer Console** window remains usable over the retail loading screen. It
queues locally typed commands onto the game thread, keeps an in-memory command history, and is
never exposed to the multiplayer server. It intentionally accepts arbitrary retail commands, so
use it only for development. Retail-log comparison established that the direct `map` path opens
`LoadingScreen.gfx` and initializes the client actor but does not send KCD2's native New Game
start message. The local-only `mp_debug_start_native_game` command invokes the signature-audited
`C_NewGameHelper` start path for diagnosis after the sandbox has loaded. It is deliberately not
automatic until its effects on the retail playline and game initialization have been verified.
That post-load experiment initializes native playline and world layers but does not close a loading
screen whose lifecycle was already missed. A pre-map invocation was also tested and did not
complete the direct-map transition, so normal joins now require a natively loaded save.
Engine output continues to appear in the diagnostic log.

The server entity-control message runs on the game thread. It records each normal entity's
active/hidden state, deactivates and hides it, applies the same rule to entities created later,
and restores the original states on enable or disconnect. The local `Dude` entity and registered
remote-player avatars are excluded. Activate/Hide VTable targets are included in the offline
signature audit.

The remote-avatar lifecycle, interpolation adapter, and fail-closed backend contract are present.
The supported retail image now has audited base EntitySystem spawn/removal entries, but still lacks
the verified KCD2 Actor/Soul initialization, AI/collision isolation, Idle/Walk/Run, equipment paths,
and a fixed human template required for the complete capability. If a second player needs an avatar,
the client therefore disconnects with an explicit diagnostic instead of using console, Lua, clone,
or invisible-player fallbacks.

Protocol version 3 adds revisioned avatar descriptors, server archetype allowlists, visible
equipment/stance fields, a reusable controlled-NPC core, a model selector, and the public Lua
`npc` API. The protocol, server, and library portions are active; the retail character backend
remains capability-gated until its native targets can be audited against the supported
`WHGame.dll`. Archetypes are human Soul UUIDs read from the local `Tables.pak`; the searchable
client catalog and versioned [JSON/Markdown catalog](docs/npc-archetypes.md) use
`763db0bb-4469-497d-bdc9-712b3df91b5a` (`ksta_additive_man_18`) as the built-in fallback.
See [Controlled NPC API and avatar protocol](docs/npc-api.md).

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

The diagnostic log console is always visible in Debug and Release; it is output-only. Startup
reports each major stage, the detected PE fingerprint, signature validation, and whether hooks
were enabled. A successful startup ends with:

```text
KCD2MP initialization completed - 67/67 signatures resolved - hooks enabled
```

Signatures can be checked without starting the game through the TUI's `Audit signatures` action.
The same native tool can be built and run from a terminal:

```powershell
cmake --build out\build\debug --config Debug --target KCD2MPSignatureAudit
out\build\debug\Debug\KCD2MPSignatureAudit.exe "C:\path\to\WHGame.dll"
```

Runtime and audit share one typed 67-entry registry and the same Zydis-based call, RIP-relative,
VTable, and PE-range validation. The supported retail image currently resolves 93/93 derived
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
