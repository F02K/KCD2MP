# KCD2MP 0.2 sandbox multiplayer status

## Implemented foundation

Protocol version 2 and KCD2MP version 0.2.0 implement the server-independent part of the
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

For Steam build `23914554` / WHGame `1308617_856`, joining from the real frontend now:

- requires the audited retail console and its `ExecuteString` VTable entry;
- resolves only allowlisted numeric level IDs to retail map names;
- records and changes typed `ICVar` values for save/load, autosave-test behavior, synchronous
  level loading, new-game intro skipping, playline freezing/deletion, and last-save autoload;
- queues the retail `map` command without accepting server-provided command text;
- waits for both the `Dude` player entity and matching `wh_sys_BaseLevelId` to remain stable
  while the native level-start sequence finishes;
- applies the persisted player transform, or the canonical server spawn for a new profile;
- sends `ClientWorldReady` only after those checks pass;
- unloads the map on final disconnect and restores the previous CVar values after the player
  entity has been released.

The transition never loads a native save and fails closed when a required CVar, command, level,
player entity, or transform is unavailable. The supported public world IDs are `2` (`trosecko`),
`3` (`kutnohorsko`), and `4` (`klaster`); additional retail test levels are allowlisted for
development.

The remaining native gaps are RPG/inventory capture and application, a dedicated fixed player
template independent of the retail map default, stronger quest/dialog scheduler isolation,
and fixed remote-human avatar creation, animation, and removal.

## Verification

Automated tests cover:

- protocol round trips, malformed/oversized messages, invalid enums and profiles;
- enrollment, duplicate identities, server capacity, claim-code rotation, restart persistence,
  profile revisions, initializer/waiter behavior, lease release, and bootstrap timeout;
- DPAPI encrypted round trip and rejection of a corrupted identity file;
- bounded game/network queues and reliable/unreliable loopback networking;
- the existing PE fingerprint and native signature registry, including the audited console
  execution VTable target.

DPAPI isolation across two different Windows user accounts, native save-directory byte/timestamp
comparison, 30-minute in-game play, two real clients, and clean exit after final disconnect remain
manual acceptance tests. In particular, the retail `map` path and default player creation must be
confirmed in-game before Version 0.2 can be called release-ready.

World deltas for containers, doors, NPCs, weather, time, and quests remain outside version 0.2.
