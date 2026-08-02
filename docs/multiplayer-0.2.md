# KCD2MP 0.3 sandbox multiplayer status (historical)

This document describes the removed pre-v4 prototype. It is retained as
development history and is not an engine-ABI reference. See the
[v4 libKCD2/KCSE migration audit](libkcd2-kcse-migration.md).

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
disable_human_npcs = false
disable_animal_npcs = false

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

The multiplayer profile is applied only to the already loaded runtime session. Automatic creation
of a separate save copy, quest/dialog scheduler state, and general world deltas remain outside the
profile contract. The complete two-client retail acceptance matrix remains a release check.

## Server entity isolation and remote avatars

`entities <all|humans|animals> <disable|enable>` and `entities status` control a reliable
server-owned `ServerEntityControl` state. `disable_human_npcs` and `disable_animal_npcs` select
the state after server start. The legacy `disable_non_player_entities` key remains accepted as a
fallback for both categories. Accepted clients and late joiners receive the same values.

On the game thread the client classifies AI Actors through the verified `C_Human` and `C_Animal`
RTTI hierarchies and applies the selected category state to existing and newly spawned NPCs. It
captures visibility before applying sink-driven `Hide` isolation. Actor activation, AI identity,
physics, animation, and combat state remain untouched. Unknown Actor subclasses, non-AI engine
helpers, the local player, and registered remote-player Entities remain active. Destroyed or reused
Entities are tracked by the audited `IEntitySystemSink` order; enable/disconnect restores every
surviving Entity symmetrically.

The remote-avatar manager consumes the existing interpolated remote-player views and implements
the complete spawn/update/remove lifecycle behind a native backend contract. Reconnecting players
remain at their last transform and `PlayerLeft` removes their avatar.

The retail backend calls native `IActorSystem::CreateActor` with the `NPC` factory, registers the
Entity as a player exception before isolation can affect it, applies the authoritative shared Soul,
and polls the readiness chain Entity → Actor → Soul → Human → Inventory. AI, perception flags, and
the concrete physical Entity proxy are disabled. Interpolated position and rotation are written
through audited `IEntity::SetWorldTM`; Idle/Walk/Run requests are driven through the native Actor
MovementController.

Local inventory and equipment are captured natively. Logical instance UUID, definition UUID,
count, quality, condition, canonical slot, stance, and Draw/Holster state are transmitted.
Receivers validate all definitions before mutation, unequip in reverse layer order, reconcile
instances, equip in layer order, and apply weapon state. Any failed mutation restores the last
confirmed state through the same reconciler; rollback failure immediately starts native world
unload.

Failed desired Souls or replacements retain the built-in Default-Soul fallback and retry after
1/2/4/8/16/30 seconds. A replacement becomes active only after it is fully ready. Backend failures
produce a concrete client error. Removal transactionally clears created equipment, unregisters the
player exception, and calls native `IEntitySystem::RemoveEntity`, including on disconnect, epoch
change, sandbox end, and external destruction. No Workshop runtime dependency, generated Lua,
console, raw EntitySystem spawn, or cloning fallback is used.

## Protocol v7 world interaction sync

Protocol v7 adds revisioned, server-authoritative state for doors and inventory-backed containers.
Open/close script events are captured by stable Entity GUID. While a container is open, its full
instance UUID, definition UUID, stack count, quality, and condition set is polled and replicated.
Conflicting observations are rejected with the complete authoritative state, and known world
objects are persisted in `world/world_objects.pb` and replayed as individual reliable messages to
late joiners so large inventories do not inflate the bootstrap packet.

Accepted player inventory ownership removes the same item instance from every persisted container
and advances its revision. A second profile cannot claim an instance already owned by another
player; profile rejection applies the authoritative inventory without disconnecting the client.
The sandbox disables door and stash lockpicking requirements, explicitly unlocks synchronized
objects when applying state, and clears stolen ownership from locally looted items. Streamed-out
objects retain their deferred authoritative state and receive it when their Entity becomes
available again.

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
- door/container revision conflicts, synchronized loot ownership, stale-container correction,
  late-join replay, and world-object restart persistence;
- DPAPI encrypted round trip and rejection of a corrupted identity file;
- bounded game/network/local-console queues and reliable/unreliable loopback networking;
- server entity-control broadcasts, reversible entity-state bookkeeping, remote-avatar primary/
  fallback/retry/cleanup behavior, and controlled-NPC external-destruction handling;
- equipment catalog slot filtering, layer order, weapon classification, and active mod-pak
  overlays; avatar UUID/slot/revision validation and non-disconnecting rejection;
- starter TOML validation, exact stat/skill sets, inventory diffs, stack changes, slot ordering,
  transactional profile rollback, stale handles, epochs, and capability masks;
- delayed Actor/Soul/Human readiness, spawn timeout, external destroy, Default-Soul fallback,
  remove in each lifecycle state, and partially failed equipment transactions;
- the exact PE fingerprint and native signature registry, including transform, sink, physics,
  item/equipment/RPG, Human weapon, and native unload targets.

DPAPI isolation across two different Windows user accounts, native save-directory byte/timestamp
comparison, 30-minute in-game play, the full two-client Soul/equipment/weapon/locomotion matrix,
fault injection, and clean exit after final disconnect remain manual acceptance tests.

World deltas for containers, doors, NPCs, weather, time, and quests remained outside version 0.3.
Protocol v7 adds doors and inventory-backed containers; NPC world state, weather, time, and quests
remain outside the current contract.

Version 0.4 defines revisioned remote-avatar descriptors, server-side archetype allowlists, and
visible equipment/stance replication. The native controlled-NPC machinery remains an internal
multiplayer component and is not exposed as a general Lua API. A complete pre-`WorldReady` active
probe publishes the runtime capability mask.
