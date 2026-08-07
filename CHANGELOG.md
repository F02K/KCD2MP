# Changelog

KCD2MP uses a single semantic project version for client, server, build
artifacts, and network compatibility. The format is `MAJOR.MINOR.PATCH`.

Because the project is still a prototype, any component may contain breaking
changes. Client and server versions must match exactly.

## [Unreleased]

### Added

- Added sequenced, non-combat Mannequin fragment replication for jumps,
  gestures, and interaction one-shots through KCD2's native `PlayAnim` path.
- Added continuous locomotion intent (local velocity, acceleration, facing,
  turn rate, strafing, and sprint classification) to player snapshots.

### Changed

- Remote transforms now use damped corrections with a teleport threshold while
  KCD2's native movement controller owns locomotion blending.
- Remote equipment reconciliation now retains unchanged native item instances
  and only removes or creates actual deltas; weapon-set transitions continue
  through the native draw/holster controller.

### Fixed

- Derived local player velocity from consecutive native transforms so remote
  avatars enter walk/run locomotion instead of remaining idle.
- Applied interpolated remote transforms at presentation cadence and reduced
  the interpolation buffer at normal snapshot rates for smoother, more
  responsive player movement.
- Moved the in-game chat onto a presentation-rate ImGui frame, separated its
  state from world/NPC synchronization, and prioritized its unbatched reliable
  packets so UI and delivery no longer wait for engine refreshes.

## [0.1.1] - 2026-08-06

### Added

- Added a localized in-game multiplayer chat overlay with input capture and
  fading recent-message history.

### Changed

- Bumped the shared client, server, protocol, resource, and package version to
  `0.1.1`.
- The generated NPC world catalog now seeds the server registry before client
  discovery; lease owners still run native AI and report routines, health,
  inventory, and dialogue state back to the server.
- Runtime NPC discovery IDs are deduplicated across reporters, and clients
  adopt an existing matching actor before attempting a managed spawn.

### Fixed

- Accepted native trading, quest/crafting, and authored-world item gains into
  the ownership ledger without rejecting and deleting the live game item.
- Made initial authored-world pickups atomic and broadcast their removal to
  other clients.
- Avoided full native inventory/equipment reconciliation when only server wire
  metadata changed, and closed on destructive ledger conflicts without
  mutating a live trading inventory.
- Serialized MSVC program-database writes for parallel KCSE native-runtime
  builds, preventing intermittent `C1041` build failures.
- Prevented uncatalogued NPC discoveries from spawning duplicate actors inside
  existing NPCs.
- Excluded stable local/remote player Entities and managed actor names from NPC
  discovery, including during temporary dialogue/cinematic Actor changes.

### Known issues

- NPC synchronization remains unreliable. A known identity/spawn bug can still
  cause the same NPC to spawn multiple times.

## [0.1.0] - 2026-08-06

### Added

- Added an install-ready dedicated-server ZIP with `start_server.bat` and a
  standalone game-data generator for GitHub Release users.

### Changed

- Bumped the shared client, server, protocol, resource, and package version to
  `0.1.0`.
- Corrected packaged game-directory paths to use `Mods`.

### Security

- Excluded the locally sourced `game_data` directory and `WHGame.dll` from the
  dedicated-server release ZIP while preserving local build-tool generation.

## [0.0.9] - 2026-08-03

### Added

- Server-authoritative synchronization for dropped items.
- Persistent dropped-item state and pickup tombstones across server restarts.
- Ownership reconciliation between player profiles, containers, and world
  items.
- Dedicated native modules for world objects and dropped items.
- Reproducible client/server/test build packages and an install-ready client
  ZIP that mirrors the KCD2 Steam directory layout.
- Coverage for regular chests, cart chests, stash corpses, bird nests, and
  destructible stashes.
- Documented server-ID, generation, authority-lease, and interest-management
  design for future human and animal NPC synchronization.

### Changed

- Replaced the separate numeric protocol and client versions with the single
  KCD2MP version `0.0.9`.
- Made the CMake project version the source for the generated handshake version.
- Aligned Windows resource metadata and nightly artifacts with `0.0.9`.
- Reworked project documentation to state prototype status and current limits.

### Fixed

- Increased the integration-test idle timeout to prevent false build/deploy
  failures during longer native verification runs.
- Prevented failed remote dropped-item creation from leaving partial inventory
  state behind.

## Before 0.0.9

Earlier prototype work used inconsistent project labels and internal numeric
protocol milestones. Those numbers were never a stable public release history.
The pre-0.0.9 foundation introduced:

- Direct-IP networking and the persistent dedicated server;
- identity enrollment, reconnect tokens, and profile recovery;
- native KCSE runtime capability checks;
- remote avatars, inventory, equipment, and weapon state;
- server-controlled human and animal NPC isolation;
- synchronized doors and inventory-backed containers; and
- shared time-of-day and weather.

Starting with `0.0.9`, all changes are recorded only against the unified KCD2MP
project version.
