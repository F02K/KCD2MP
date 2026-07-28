# KCD2MP 0.3 sandbox multiplayer status

## Implemented foundation

Protocol version 3 and KCD2MP version 0.3.0 implement the server-independent part of the
isolated sandbox flow:

1. `ClientHello`
2. `ServerChallenge`
3. `ClientAuthenticate`
4. `ServerBootstrap`
5. `ClientWorldReady` or `ClientWorldFailed`
6. `ServerAccepted`

Only ping/pong is accepted while a client is loading. Transforms, chat, lifecycle replication,
and profile updates are accepted only after `ServerAccepted`.

The dedicated server persists:

- `world/session.toml`: schema, permanent server and session IDs, level, content hash, random
  seed, manifest revision, next player ID, and canonical spawn;
- `world/players/<player_id>.pb`: SHA-256 identity-token hash, profile revision, last accepted
  transform, money, stats, skills, inventory, and equipment slots.

Files are written to a temporary sibling and replaced atomically with write-through semantics.
The server never stores, reads, or transports a native KCD2 save.

Identity tokens contain 256 random bits. They are returned once, stored by the client in a
separate `%LOCALAPPDATA%\KCD2MP\multiplayer-identities.bin` file protected with Windows DPAPI,
and stored server-side only as a Windows BCrypt SHA-256 hash. `profile claim <player_id>`
creates a one-use 128-bit recovery code valid for ten minutes and rotates the identity token
after successful authentication.

## Session initialization and persistence

If `[server.initial_spawn]` exists, it becomes the canonical spawn when the world directory is
created. Otherwise exactly one authenticated client receives `BOOTSTRAP_MODE_INITIALIZE`.
Other first-time clients wait. A validated engine-default spawn increments the manifest
revision and releases all waiters; disconnecting or timing out releases the initialization
lease.

Player IDs survive server restarts. A persistent identity may only be connected once. Profiles
carry an optimistic revision; invalid schemas and stale revisions receive `ProfileRejected` and
the connection is closed to avoid accepting a potentially duplicated inventory state.
Transforms are persisted every five seconds and during disconnect/shutdown.
The client-side revision/acknowledgement pipeline schedules a profile snapshot at the configured
15-second interval and before intentional disconnect. While the native RPG/inventory capture
gate is unavailable it returns no snapshot instead of fabricating or reading save data.

The profile schema limits each player to 128 stats, 128 skills, and 512 inventory instances.
Identifiers, UTF-8 text, finite numeric values, counts, quality, condition, and total protobuf
message size are validated. Quest items, perks, reputation, crimes, horses, buffs, health,
nutrition, and fatigue are intentionally not represented.

## Server configuration

Start from `server.toml.example`. Relative world paths are resolved next to the selected
configuration file.

```toml
[server]
bind_address = "0.0.0.0"
port = 27020
name = "KCD2MP Sandbox"
password = ""
max_players = 8
level_id = "3"
required_content_hash = ""
world_directory = "world"
bootstrap_timeout_seconds = 180
profile_snapshot_interval_seconds = 15
disable_non_player_entities = false

# Optional:
# [server.initial_spawn]
# x = 0.0
# y = 0.0
# z = 0.0
# qx = 0.0
# qy = 0.0
# qz = 0.0
# qw = 1.0
```

Do not copy a world directory between unrelated servers unless the shared server/session
identity and all player identities are intentionally meant to move together.

## Native safety gate

The dedicated server, protocol, persistence, networking, and DPAPI identity layer compile and
run without KCD2. The in-game client now has an experimental signature-gated retail transition.

For Steam build `23914554` / WHGame `1308617_856`, normal joining requires a save loaded through
KCD2. The bootstrap:

- requires the audited retail console and its `ExecuteString` VTable entry;
- resolves only allowlisted numeric level IDs to retail map names;
- records and changes typed `ICVar` values for save/load, autosave-test behavior, synchronous
  level loading, retail intro-video hiding, playline freezing/deletion, and last-save autoload;
- adopts the already running save only when its live base-level ID matches the server,
  without reading, copying, modifying, or uploading the `.whs` file;
- waits for both the `Dude` player entity and matching `wh_sys_BaseLevelId` to remain stable
  while the native level-start sequence finishes;
- applies the persisted player transform, or the canonical server spawn for a new profile;
- sends `ClientWorldReady` only after those checks pass;
- closes the mod GUI after `ServerAccepted` so its input layer releases mouse and keyboard control
  to the game, unless the local developer console was deliberately left open;
- unloads the direct-map or adopted save world on final disconnect and restores the previous CVar
  values after the player entity has been released.

KCD2MP never invokes a native save load itself: the user completes that operation through KCD2
before connecting. The transition fails closed when a required CVar, command, level, player entity,
or transform is unavailable. The supported public world IDs are `2` (`trosecko`), `3`
(`kutnohorsko`), and `4` (`klaster`); additional retail test levels are allowlisted for
development.

The remaining native gaps are automatic creation/loading of an isolated multiplayer save copy,
RPG/inventory capture and application, stronger quest/dialog scheduler isolation, and fixed
remote-human avatar creation, animation, and removal.

## Server entity isolation and remote avatars

`entities disable`, `entities enable`, and `entities status` control a reliable server-owned
`ServerEntityControl` state. `disable_non_player_entities` selects the state after server start.
Accepted clients and late joiners receive the same value.

On the game thread the client preserves every normal `CEntity`'s active and hidden state before
calling the audited `Activate(false)` and `Hide(true)` VTable entries. The local `Dude`, registered
remote-player entities, and their later replacements are excluded. Newly created normal entities
inherit the disabled state, destroyed entities are forgotten, and enable/disconnect restores the
per-entity values.

The remote-avatar manager consumes the existing interpolated remote-player views and implements
the complete spawn/update/remove lifecycle behind a native backend contract. Reconnecting players
remain at their last transform and `PlayerLeft` removes their avatar. The backend deliberately
remains unavailable for the supported retail image: no fixed human template or audited native
Actor/Soul initialization, AI/collision/damage isolation, Idle/Walk/Run, and equipment path
exists in the repository. The base EntitySystem spawn/removal entries themselves are audited.
When a remote transform first requires an avatar, the client disconnects with a diagnostic rather
than silently rendering an invisible player or falling back to console commands, Lua, or cloning.

## Local developer console

**GUI -> Developer Console** opens a local-only ImGui command prompt that remains available over
the loading screen. It accepts arbitrary commands, keeps at most 100 history entries in memory,
queues at most 64 pending commands, and executes no more than eight per game-thread tick through
the audited retail `ExecuteString` wrapper. No command history is persisted or transported over
the network.

Retail-log comparison has ruled out the generic loading screen, a normal fader, game pause, and
client-actor initialization as the cause of the incomplete transition. The direct `map` path
opens `LoadingScreen.gfx` and initializes the action map, but unlike a native load it does not
complete the Warhorse New Game/session path. After the sandbox reaches `Connected`, the
developer-only `mp_debug_start_native_game` command can invoke the signature-audited
`C_NewGameHelper` start function. The wrapper validates the live `C_Game` instance and helper
VTable before calling it. Runtime testing confirms that a post-load invocation initializes
playline, quest, and world layers but arrives too late to complete the loading-screen lifecycle.
A pre-map invocation was also tested. Neither ordering completes the direct-map loading screen, so
normal joins now adopt a save that KCD2 loaded through its native UI. Command execution details
remain in the diagnostic log.

## Verification

Automated tests cover:

- protocol round trips, malformed/oversized messages, invalid enums and profiles;
- enrollment, duplicate identities, server capacity, claim-code rotation, restart persistence,
  profile revisions, initializer/waiter behavior, lease release, and bootstrap timeout;
- DPAPI encrypted round trip and rejection of a corrupted identity file;
- bounded game/network/local-console queues and reliable/unreliable loopback networking;
- server entity-control broadcasts, reversible entity-state bookkeeping, and remote-avatar
  lifecycle behavior through fake native backends;
- the existing PE fingerprint and native signature registry, including the audited console
  execution VTable target.

DPAPI isolation across two different Windows user accounts, native save-directory byte/timestamp
comparison, 30-minute in-game play, two real clients, and clean exit after final disconnect remain
manual acceptance tests. In particular, native remote-character creation must be confirmed
in-game before Version 0.3 can be called release-ready.

World deltas for containers, doors, NPCs, weather, time, and quests remain outside version 0.3.

Version 0.3 additionally defines revisioned remote-avatar descriptors, server-side archetype
allowlists, visible equipment/stance replication, and an engine-neutral controlled-NPC/Lua API.
The native retail character backend remains behind the same fail-closed safety gate until the
complete Actor/Soul initialization, isolation, animation, and equipment lifecycle is audited.
