# Changelog

KCD2MP uses a single semantic project version for client, server, build
artifacts, and network compatibility. The format is `MAJOR.MINOR.PATCH`.

Because the project is still a prototype, any component may contain breaking
changes. Client and server versions must match exactly.

## [Unreleased]

- No unreleased changes documented yet.

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
