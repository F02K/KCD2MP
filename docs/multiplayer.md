# Multiplayer architecture and prototype status

This document describes KCD2MP **v0.0.9**. The implementation is an active
prototype and is not intended for production servers or valuable saves.

## Versioning and compatibility

KCD2MP has one semantic project version shared by:

- the native client;
- the dedicated server;
- Windows build metadata;
- packaged artifacts; and
- the multiplayer handshake.

The current version is `0.0.9`. There is no separate public protocol version.
During the prototype phase, client and server versions must match exactly. A
mismatch is rejected before authentication or world loading begins.

Internal safety counters such as the KCSE C ABI revision, persistence schema,
and Address Library format remain independently versioned. They are
implementation compatibility checks, not KCD2MP release versions.

Wire messages still evolve with the project, but their compatibility boundary
is the KCD2MP version. This avoids a numeric wire version carrying a different,
unrelated application version. Version changes are recorded in the
project [changelog](../CHANGELOG.md).

## Connection lifecycle

A normal connection follows this sequence:

1. `ClientHello`
2. `ServerChallenge`
3. `ClientAuthenticate`
4. `ServerBootstrap`
5. `ClientWorldReady` or `ClientWorldFailed`
6. `ServerAccepted`

`ClientHello` contains the KCD2MP version and native runtime descriptor. The
server verifies the game fingerprint, KCSE/libKCD2 release, Address Library,
required capabilities, level, and content hash. Only control traffic is
accepted while the client loads; gameplay updates start after
`ServerAccepted`.

The client loads a save through KCD2's own UI before joining. KCD2MP adopts the
already loaded world only when its level matches the server. It does not load,
copy, upload, or modify native save files.

## Server authority and persistence

The dedicated server owns the canonical multiplayer state. Its
`world_directory` contains:

- `session.toml` for server/session identity, world configuration, spawn, and
  revision metadata;
- `players/<player_id>.pb` for authenticated player profiles;
- `world_objects.pb` for doors and synchronized containers; and
- `world_items.pb` for dropped-item state and tombstones.

Persistence uses a temporary sibling followed by atomic replacement. Player
IDs, object identities, item instance UUIDs, and revisions survive restarts.

Identity tokens contain 256 random bits. The client stores them with Windows
DPAPI; the server stores only their SHA-256 hashes. Recovery codes are
single-use, expire after ten minutes, and are printed only when explicitly
requested by an administrator.

## Player profiles

An authoritative player profile currently includes:

- money;
- 10 canonical stats;
- 35 skills and progress values;
- inventory instances;
- quick-access assignments;
- equipment slots; and
- avatar appearance and weapon state.

Profiles use optimistic revisions. Malformed values, stale revisions, invalid
definitions, or duplicate instance ownership are rejected. The client applies
the returned canonical state through the same native reconciler used for
capture. Failed mutations roll back; a failed rollback initiates a safe world
unload.

Quests, dialogue, reputation, crimes, perks, horses, buffs, nutrition, fatigue,
and general savegame state are outside the current profile contract.

## Remote players

Remote players are represented by native actors managed behind the KCSE runtime
boundary. The lifecycle is:

1. allocate the representation;
2. register it as an isolation exception;
3. wait for Entity, Actor, Soul, Human, and Inventory readiness;
4. apply appearance and equipment;
5. consume interpolated movement snapshots; and
6. transactionally remove all native state on leave or disconnect.

Movement uses audited native transforms and the Actor movement controller.
Equipment changes are ordered, validated, and recoverable. Failed Soul
replacements retain the default fallback and retry with bounded backoff.

## Doors and loot containers

Doors and containers use a revisioned, server-authoritative world-object stream
keyed by stable Entity GUID. Their native implementation is isolated in
`native_world_object_sync.*`.

Container discovery covers:

- regular `Stash` containers and chests;
- `CartStash`;
- `StashCorpse`;
- bird nests; and
- destructible `ShootableStashBase` variants.

While a container is open, its complete item set is captured with instance and
definition UUID, stack count, quality, and condition. Revision conflicts return
the canonical state. Accepting an item into a player profile removes the same
instance from synchronized containers and advances their revisions.

Unknown future script classes are not assumed to be compatible merely because
they are visually chest-like. They require an inventory accessor matching one
of the supported native paths or an explicit adapter.

## Dropped items

Version `0.0.9` adds a separate world-item stream keyed by the persistent item
instance UUID. Local CryEngine Entity IDs are never used as network identity,
because each game process creates its own visual/pickable Entity.

Each canonical world item contains:

- instance and definition UUID;
- count, quality, and condition;
- position and rotation;
- presence/tombstone state; and
- revision.

At synchronization start, the native client records pickups already present in
the loaded save as a local baseline. Those items remain local save content and
are not uploaded as multiplayer drops. Once a baseline item enters the local
player inventory, a later drop can become multiplayer-managed.

Remote creation uses the game's Human `PlaceItem` path so the normal world
inventory and pickable extension are established. Failed creation is rolled
back instead of leaving a partial item in the player's inventory. Pickup emits
a tombstone, and dropping the same UUID again reactivates its canonical record
instead of creating another identity.

Ownership transfer is instance-atomic at the server boundary:

- dropping removes the instance from the sender profile and any synchronized
  container before exposing it in the world;
- pickup tombstones the world representation when the profile claim succeeds;
  and
- a second player cannot retain an instance already owned elsewhere.

## Environment

The server owns the time-of-day anchor, real-time anchor, time scale, weather,
and associated revisions. Clients advance time locally and apply bounded drift
corrections. Weather transitions use a separate revision so they are not
restarted on every snapshot.

Time values are applied through the engine CVar path. Weather continues through
the game's weather command so native profile blending remains intact.

## NPCs: current state

Shared NPC simulation is not implemented in `0.0.9`. The server can instruct
clients to isolate human NPCs, animal NPCs, or both. Classification uses native
`C_Human` and `C_Animal` RTTI; unknown Actors and non-AI helpers are not hidden.

Isolation prevents each client from independently presenting and simulating its
own unsynchronized population. It is a prototype safety measure, not NPC sync.

## NPCs: planned synchronization model

The planned model prevents duplicate NPCs through four rules.

### Canonical identity

Every logical NPC receives a server-owned ID and generation. Authored map NPCs
are adopted through a stable catalog/spawn identity. Dynamic NPCs receive a
server-allocated ID. Local Entity IDs and pointers never leave the client.

### Idempotent local representation

Each client maintains exactly one mapping from canonical ID to local native
handle. `ensure_spawned` creates only when no valid representation exists.
Repeated enter/update messages reuse the existing Entity. A newer generation
removes stale state before replacement.

### Single simulation authority

At most one client holds a time-limited simulation lease for an NPC. That
client may submit movement and behavior observations; all other clients render
the server-approved state. The server remains authoritative for lifecycle,
generation, health, and inventory, and revokes or reassigns a lease when its
owner disconnects or leaves the relevant area.

### Spatial interest management

The server sends enter, update, and leave events only for nearby interest cells.
Each viewing client still needs one local visual Entity, but player count does
not multiply the number of logical NPCs or simultaneous AI authorities.

Humans and animals can share identity, transform, health, generation, lease,
and interest messages. Their behavior, locomotion, animation, and combat
payloads should remain type-specific.

## Native safety boundary

The in-game client performs an active capability probe before publishing
readiness. It verifies transform access, Actor/Soul/Human/Inventory readiness,
item lifecycle operations, equipment transactions, Entity cleanup, runtime
epoch, and Address Library identity.

Game objects are accessed only on the game thread. Networking uses bounded
queues and plain protocol values. LoadGame, SaveGame, NewGame, and DataLoaded
events advance the runtime epoch and invalidate stale handles.

The client fails closed when a required native capability is missing. Removed
signature, generated-Lua, console, and guessed-name fallbacks are not used for
multiplayer state mutation.

## Automated verification

The test suites cover:

- handshake/version validation and malformed or oversized messages;
- authentication, identity recovery, capacity, reconnect, and timeouts;
- profile revisions, inventory ownership, reconciliation, and rollback;
- remote-avatar lifecycle, readiness, fallback, and cleanup;
- door/container conflicts, persistence, and late-join replay;
- dropped-item revisions, ownership transfer, tombstones, and restart replay;
- environment validation and updates;
- bounded queues and reliable/unreliable networking;
- Address Library coverage and native signature resolution; and
- build-tool discovery, auditing, and atomic deployment.

## Known prototype limitations

- No synchronized human or animal NPC world simulation
- No shared quests, dialogue, crime, reputation, or story progression
- No compatibility promise between different KCD2MP versions
- One explicitly supported Steam/WHGame build
- Direct-IP hosting without matchmaking or relay service
- Remaining multi-client gameplay and long-duration checks require manual
  in-game validation

These limitations are release blockers, not hidden production features.
