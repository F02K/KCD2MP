# LuaUtils

KCSE plugin for KCD2 that extends the vanilla `ItemManager` Lua table and publishes a new global `EquipmentManager` table.

All functions use **dot-call syntax**:

```lua
ItemManager.GetItemEx(itemId)
EquipmentManager.GetEquippedClothing(entityId)
```

Do not call these functions with `:`. Item, inventory, and owner IDs are passed as Lua `ScriptHandle` values so the full 64-bit Warhorse WUID is preserved.

## ItemManager additions

### Queries

| Function | Returns |
| --- | --- |
| `ItemManager.GetItemEx(itemId)` | `{ id, className, type, amount, health, condition, quality, maxQuality, isEquipped, owner, entity }` |
| `ItemManager.IsItemEquipped(itemId)` | `bool` — per-instance equipped flag (`C_Item` flags bit 0) |
| `ItemManager.GetItemCondition(itemId)` | `float 0..1` — effective durability; runtime-data aware for equippables |
| `ItemManager.GetItemQuality(itemId)` | Current integer quality |
| `ItemManager.GetItemMaxQuality(itemId)` | Maximum integer quality |
| `ItemManager.GetItemPrices(itemId)` | `{ unit, stack, newUnit, newStack }`; current and new-condition unit/stack prices |

### Mutators

Mutators return `true` on success and `nil` if resolution or the native operation fails, except `MoveItem`, which returns the surviving item WUID.

| Function | Notes |
| --- | --- |
| `ItemManager.SetItemHealth(itemId, health)` | Native `SetItemHealth`, clamped to `0..1`, with the engine notify path |
| `ItemManager.SetItemCondition(itemId, condition)` | Runtime-data condition for equippables; health for other item types |
| `ItemManager.SetItemQuality(itemId, quality)` | Equippable-type items only |
| `ItemManager.SetItemAmount(itemId, amount)` | Listener-correct amount change; `amount <= 0` deletes a parented stack |
| `ItemManager.SetItemOwner(itemId, ownerId[, stolenFromOwnerId])` | Native owner write and owner-index update; omitted theft-source ID defaults to `ownerId` |
| `ItemManager.WashItem(itemId[, maxEffect])` | Per-item dirt operation; omitted effect performs a full wash |
| `ItemManager.SetItemPhaseId(itemId, phaseId)` | Set the discrete food/torch phase |
| `ItemManager.SetItemPhase(itemId, phase)` | Set normalized phase progress |
| `ItemManager.AdvanceItemPhase(itemId, amount)` | Advance normalized phase progress |
| `ItemManager.MoveItem(itemId, destinationInventoryId[, count])` | Split/merge-aware native move; `count` absent or `0` means the whole stack |

`ownerId` and `stolenFromOwnerId` are WUIDs, not CryEngine entity IDs. Theft state is not reducible to a permanent `ownerId != stolenFromOwnerId` test: the native system also records when the owner mark was set and can age or fade that mark.

## EquipmentManager

This is a new global table keyed by CryEngine entity ID.

| Function | Returns / behavior |
| --- | --- |
| `EquipmentManager.GetEquippedItems(entityId)` | Dense array of materialized equipped item WUIDs |
| `EquipmentManager.GetEquippedClothing(entityId)` | `{ [equipmentSlotId] = itemId }` for data-driven clothing slots |
| `EquipmentManager.GetHandSlots(entityId)` | Sparse `{ [1..8] = itemId }` using the fixed native weapon-equip mapping below; empty entries are absent |
| `EquipmentManager.GetItemInSlot(entityId, slotId)` | Item WUID in a data-driven clothing slot, or `nil`; does not query weapon or QAM slots |
| `EquipmentManager.GetEquipWeights(entityId)` | `{ total, worn }` |
| `EquipmentManager.GetInventoryEx(entityId)` | Dense array of `GetItemEx`-shaped item tables |
| `EquipmentManager.GetInventoryId(entityId)` | Inventory WUID usable with `MoveItem` |
| `EquipmentManager.SetItemEquipped(entityId, itemId, equip)` | Equip/unequip through the entity's inventory soul, preserving native visuals and bookkeeping |

### `GetHandSlots` mapping

Despite its compatibility name, `GetHandSlots` exposes the complete fixed `E_WeaponEquipSlot` array, not only literal hands:

| Lua key | Native value | `E_WeaponEquipSlot` |
| ---: | ---: | --- |
| `1` | `0` | `PrimaryMainHand` |
| `2` | `1` | `PrimaryOffHand` |
| `3` | `2` | `SecondaryMainHand` |
| `4` | `3` | `SecondaryOffHand` |
| `5` | `4` | `Oversized` |
| `6` | `5` | `OversizedOff` |
| `7` | `6` | `Torch` |
| `8` | `7` | `Dagger` |

## Clothing slots, weapon slots, and QAM are separate

KCD2 has three independent slot systems:

1. **Clothing `EquipmentSlotId`** — a data-driven 32-bit database ID. `GetEquippedClothing` and `GetItemInSlot` use this system.
2. **`E_WeaponEquipSlot`** — the fixed eight-entry native array exposed by `GetHandSlots`.
3. **Player QAM assignments** — outfit-specific quick-access assignments held by the player inventory soul. They are not entries in either table above.

The fixed four-byte `S_EquipmentSlotIdWrapper` does not make clothing slots a compiled enum. Only the wrapper ABI is fixed; valid IDs and names come from the `equipment_slot` database.

### Belt and pouch

| Identifier domain | Belt | Pouch |
| --- | ---: | ---: |
| Clothing `EquipmentSlotId` | `44` | `45` |
| `ArmorType` | `76` | `77` |
| `ArmorArchetypeId` used by QAM capacity logic | `103` | `104` |

The equipped container items can therefore be queried as clothing:

```lua
local beltItem = EquipmentManager.GetItemInSlot(entityId, 44)
local pouchItem = EquipmentManager.GetItemInSlot(entityId, 45)
```

Those calls return the **belt or pouch item**, not the weapon, potion, food, or consumable assigned to quick access.

- An equipped belt controls the usable weapon-QAM pair count. The native no-belt baseline is one weapon pair.
- An equipped pouch controls the usable food/consumable-QAM slot count. The native no-pouch baseline is zero food slots.
- QAM assignments are stored separately for outfits A, B, and C in player-specific outfit managers.

LuaUtils currently exports **no QAM assignment query or mutator**. The storage and slot enums are recovered, but the complete QAM-manager interface is not yet declared to the project's source-accuracy standard. `GetHandSlots` is not a substitute; it reports physical weapon-equip slots only.

## Examples

```lua
-- Which of the bandit's duplicate mailles is actually equipped?
local bandit = System.GetEntityByName("bandit_01")
for _, item in ipairs(EquipmentManager.GetInventoryEx(bandit.id) or {}) do
    if item.className == "maille_long" then
        System.LogAlways(tostring(item.id) .. " equipped=" .. tostring(item.isEquipped))
    end
end

-- Which clothing item occupies each merchant equipment slot?
local merchant = System.GetEntityByName("merchant_01")
local worn = EquipmentManager.GetEquippedClothing(merchant.id)

-- Query the player's equipped belt and pouch container items.
local belt = EquipmentManager.GetItemInSlot(player.id, 44)
local pouch = EquipmentManager.GetItemInSlot(player.id, 45)

-- Repair and clean the player's current main-hand item.
local itemId = player.human:GetItemInHand(0)
ItemManager.SetItemHealth(itemId, 1.0)
ItemManager.WashItem(itemId)
```

## Resolution caveats

- Distant or unstreamed NPCs may not yet have their worn items materialized as `C_Item` instances. Queries can therefore return less than the NPC conceptually owns; interact with or approach the NPC first.
- Some souls have no inventory or equipment manager. All query functions are nil-graceful instead of raising Lua errors.
- Direct stat writes use the engine's notify paths, but some UI screens refresh only after being reopened.
- `SetItemQuality` returns `nil` for non-equippable item types.

## Build

LuaUtils is part of the KCD2 RE workspace. Build the `release` preset from `KCD2/RE/.buildenv`; the DLL is emitted under `build-release/LuaUtils/LuaUtils.dll` and deploys like other KCSE plugins.
