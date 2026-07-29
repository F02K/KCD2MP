#include "ResolveHelpers.h"

#include "entitymodule/C_Actor.h"
#include "entitymodule/C_EquipmentManager.h"
#include "entitymodule/C_Inventory.h"
#include "entitymodule/C_Item.h"
#include "entitymodule/C_ItemManager.h"
#include "framework/WUID.h"
#include "game/S_GameContext.h"
#include "rpgmodule/C_Soul.h"

namespace luautils {

wh::entitymodule::C_Item* ResolveItem(uint64_t itemId)
{
    auto* mgr = wh::entitymodule::C_ItemManager::GetInstance();
    if (!mgr)
        return nullptr;
    wh::framework::WUID wuid{ itemId };
    return mgr->LookupByWUID(wuid);
}

wh::entitymodule::C_Actor* ResolveActor(uint64_t entityId)
{
    auto* ctx = wh::game::S_GameContext::GetInstance();
    return ctx ? ctx->GetActorById((EntityId)entityId) : nullptr;
}

wh::rpgmodule::C_InventorySoul* ResolveInventorySoul(wh::entitymodule::C_Actor* actor)
{
    return (actor && actor->m_pSoul) ? &actor->m_pSoul->m_inventorySoul : nullptr;
}

wh::entitymodule::C_EquipmentManager* ResolveEquipment(wh::entitymodule::C_Actor* actor)
{
    auto* invSoul = ResolveInventorySoul(actor);
    return invSoul ? invSoul->GetEquipmentManager() : nullptr;
}

wh::entitymodule::C_Inventory* ResolveInventory(wh::entitymodule::C_Actor* actor)
{
    return actor ? actor->GetInventory() : nullptr;
}

}  // namespace luautils
