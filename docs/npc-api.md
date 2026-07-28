# Controlled NPC API

KCD2MP 0.3 contains an engine-neutral controlled-NPC manager and a Lua API.
NPCs created through this API are visual, locally owned actors. AI, dialogue,
quests, damage, collision, and save persistence are intentionally outside the
contract.

The native retail backend remains capability-gated. The base EntitySystem
spawn/removal entries are audited, but `npc.spawn` still returns an error until
the KCD2 Actor/Soul initialization, animation, equipment, and isolation paths
are verified as one complete lifecycle. KCD2MP does not fall back to console
commands, game Lua, or memory cloning.

## Lua

```lua
local handle, err = npc.spawn({
    archetype = "763db0bb-4469-497d-bdc9-712b3df91b5a",
    position = { x = 0.0, y = 0.0, z = 0.0 },
    rotation = { x = 0.0, y = 0.0, z = 0.0, w = 1.0 },
    locomotion = "idle",
    appearance = {
        stance = "relaxed",
        weapon_class = "none",
        weapon_drawn = false,
        equipment = {},
    },
})

if not handle then
    log.warning(err)
    return
end

local state, diagnostic = npc.status(handle)
npc.set_transform(handle, {
    position = { x = 1.0, y = 2.0, z = 3.0 },
    rotation = { x = 0.0, y = 0.0, z = 0.0, w = 1.0 },
})
npc.set_locomotion(handle, "walk")
npc.remove(handle)
```

Valid locomotion values are `idle`, `walk`, and `run`. Valid weapon classes
are `none`, `one_handed`, `two_handed`, `polearm`, `bow`, and `crossbow`.
Stance is either `relaxed` or `ready`. At most 32 visible equipment entries
are accepted.

Handles are owned by the plugin that created them. All of a plugin's handles
are queued for removal when the plugin is cleaned up or hot-reloaded.

## Human Soul catalog

`archetype` is a concrete human Soul UUID. The client deterministically reads
all human Soul rows from the local `Data/Tables.pak`; animal Soul archetypes are
excluded. The supported `1308617_856` catalog contains 7,266 entries and is
checked against a versioned fingerprint before it is exposed to multiplayer.
The generated [JSON](../data/npc_archetypes.json) and
[Markdown overview](npc-archetypes.md) are produced by
`tools/generate_npc_catalog.py`.

The built-in fallback is `763db0bb-4469-497d-bdc9-712b3df91b5a`
(`ksta_additive_man_18`, `char_GENERIC_MAN_COMMONER_18`). Unknown configured,
stored, or network-provided Soul IDs are normalized to that fallback. The
server allowlist is deduplicated after normalization.

## Multiplayer avatar contract

The server owns an archetype allowlist:

```toml
default_avatar_archetype = "763db0bb-4469-497d-bdc9-712b3df91b5a"
allowed_avatar_archetypes = ["763db0bb-4469-497d-bdc9-712b3df91b5a"]
```

Avatar descriptors are sent reliably and revisioned independently from
movement snapshots. `ServerAccepted` and `PlayerJoined` include the full
descriptor; regular `WorldSnapshot` messages contain only dynamic movement.
The owning client may submit at most four avatar changes per second.

The server validates equipment limits, unique equipment slots, stance, weapon
class, and the base revision. A stale revision receives the authoritative
descriptor. Unknown but structurally valid Soul IDs receive the normalized
authoritative descriptor. Invalid content is rejected. Player movement
continues to use the client-originated, server-validated transform pipeline.

The client canonicalizes equipment by slot and coalesces visual changes to at
most four reliable updates per second. Remote transforms and locomotion are
applied continuously; equipment, stance, weapon class, and drawn state are
applied only when the avatar revision changes. A Soul change spawns the
replacement before removing the old handle.

The current retail backend deliberately reports unavailable, so multiplayer
still fails closed when a remote model would be required. This prevents
invisible players while the remaining native character targets are unaudited.
