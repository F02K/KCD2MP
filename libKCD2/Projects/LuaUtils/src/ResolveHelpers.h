// Shared id -> engine-object resolution, nil-graceful at every hop.

#pragma once

#include <cstdint>

namespace wh::entitymodule {
class C_Actor;
class C_EquipmentManager;
class C_Inventory;
class C_Item;
}
namespace wh::rpgmodule { class C_InventorySoul; }

namespace luautils {

// itemId (WUID, tag 2) -> C_Item* via the C_ItemManager registry.
wh::entitymodule::C_Item* ResolveItem(uint64_t itemId);

// entityId -> C_Actor* via S_GameContext::GetActorById (CryAction actor map).
wh::entitymodule::C_Actor* ResolveActor(uint64_t entityId);

// actor -> &soul.m_inventorySoul (null when the soul is absent).
wh::rpgmodule::C_InventorySoul* ResolveInventorySoul(wh::entitymodule::C_Actor* actor);

// actor -> equipment manager via I_InventorySoul::GetEquipmentManager [1].
wh::entitymodule::C_EquipmentManager* ResolveEquipment(wh::entitymodule::C_Actor* actor);

// actor -> item inventory (actor+0x668 soul -> inventory soul -> GetInventory [0]).
wh::entitymodule::C_Inventory* ResolveInventory(wh::entitymodule::C_Actor* actor);

}  // namespace luautils
