#include "scriptbind/ScriptBind_EquipmentManager.h"

#include "LuaHelpers.h"
#include "ResolveHelpers.h"

#include "Offsets/vtables/IFunctionHandler.h"
#include "Offsets/vtables/IScriptSystem.h"
#include "Offsets/vtables/IScriptTable.h"
#include "entitymodule/C_Actor.h"
#include "entitymodule/C_EquipmentManager.h"
#include "entitymodule/C_Inventory.h"
#include "entitymodule/C_Item.h"
#include "rpgmodule/C_InventorySoul.h"

using namespace wh::entitymodule;

namespace luautils {

void CScriptBind_EquipmentManager::Init(Offsets::IScriptSystem* pSS)
{
    m_pSS = pSS;
    m_pMethodsTable = pSS->CreateTable(0, 0);
    if (!m_pMethodsTable)
        return;
    m_pMethodsTable->AddRef();   // CreateTable returns refcount 0; pinned for the session

    RegisterFunction("GetEquippedItems", "entityId", functor(*this, &CScriptBind_EquipmentManager::GetEquippedItems));
    RegisterFunction("GetEquippedClothing", "entityId", functor(*this, &CScriptBind_EquipmentManager::GetEquippedClothing));
    RegisterFunction("GetHandSlots", "entityId", functor(*this, &CScriptBind_EquipmentManager::GetHandSlots));
    RegisterFunction("GetItemInSlot", "entityId, slotId", functor(*this, &CScriptBind_EquipmentManager::GetItemInSlot));
    RegisterFunction("GetEquipWeights", "entityId", functor(*this, &CScriptBind_EquipmentManager::GetEquipWeights));
    RegisterFunction("GetInventoryEx", "entityId", functor(*this, &CScriptBind_EquipmentManager::GetInventoryEx));
    RegisterFunction("GetInventoryId", "entityId", functor(*this, &CScriptBind_EquipmentManager::GetInventoryId));
    RegisterFunction("SetItemEquipped", "entityId, itemId, equip", functor(*this, &CScriptBind_EquipmentManager::SetItemEquipped));

    m_pSS->SetGlobalAny("EquipmentManager", ScriptAnyValue(m_pMethodsTable));
}

void CScriptBind_EquipmentManager::RegisterFunction(const char* sName, const char* sParams, const FunctionFunctor& f)
{
    SUserFunctionDesc fd;
    fd.sGlobalName     = "EquipmentManager";
    fd.sFunctionName   = sName;
    fd.sFunctionParams = sParams;
    fd.pFunctor        = f;
    m_pMethodsTable->AddFunction(fd);
}

// ---- shared param helpers -------------------------------------------------

static C_Actor* GetActorParam(Offsets::IFunctionHandler* pH)
{
    uint64_t id = 0;
    if (!GetHandleParam(pH, 1, id))
        return nullptr;
    return ResolveActor(id);
}

// ---- queries --------------------------------------------------------------

int CScriptBind_EquipmentManager::GetEquippedItems(Offsets::IFunctionHandler* pH)
{
    C_Actor* actor = GetActorParam(pH);
    C_EquipmentManager* em = ResolveEquipment(actor);
    if (!em)
        return pH->EndFunction();
    Offsets::IScriptTable* t = m_pSS->CreateTable(0, 0);
    if (!t)
        return pH->EndFunction();
    int idx = 1;
    for (C_Item* item : em->m_equippedItems) {
        if (item)
            t->SetAtAny(idx++, HandleValue(item->m_wuid.m_value));
    }
    int n = pH->EndFunctionAny(ScriptAnyValue(t));
    t->Release();
    return n;
}

int CScriptBind_EquipmentManager::GetEquippedClothing(Offsets::IFunctionHandler* pH)
{
    C_Actor* actor = GetActorParam(pH);
    C_EquipmentManager* em = ResolveEquipment(actor);
    if (!em)
        return pH->EndFunction();
    Offsets::IScriptTable* t = m_pSS->CreateTable(0, 0);
    if (!t)
        return pH->EndFunction();
    for (const auto& [slotId, itemWuid] : em->m_clothing)
        t->SetAtAny((int)slotId, HandleValue(itemWuid.m_value));
    int n = pH->EndFunctionAny(ScriptAnyValue(t));
    t->Release();
    return n;
}

int CScriptBind_EquipmentManager::GetHandSlots(Offsets::IFunctionHandler* pH)
{
    C_Actor* actor = GetActorParam(pH);
    C_EquipmentManager* em = ResolveEquipment(actor);
    if (!em)
        return pH->EndFunction();
    Offsets::IScriptTable* t = m_pSS->CreateTable(0, 0);
    if (!t)
        return pH->EndFunction();
    for (int i = 0; i < 8; ++i) {
        if (em->m_weaponEquipSlots[i])
            t->SetAtAny(i + 1, HandleValue(em->m_weaponEquipSlots[i]->m_wuid.m_value));
    }
    int n = pH->EndFunctionAny(ScriptAnyValue(t));
    t->Release();
    return n;
}

int CScriptBind_EquipmentManager::GetItemInSlot(Offsets::IFunctionHandler* pH)
{
    C_Actor* actor = GetActorParam(pH);
    C_EquipmentManager* em = ResolveEquipment(actor);
    int slotId = 0;
    if (!em || !pH->GetParam(2, slotId))
        return pH->EndFunction();
    auto it = em->m_clothing.find((uint32_t)slotId);
    if (it == em->m_clothing.end())
        return pH->EndFunction();
    return pH->EndFunctionAny(HandleValue(it->second.m_value));
}

int CScriptBind_EquipmentManager::GetEquipWeights(Offsets::IFunctionHandler* pH)
{
    C_Actor* actor = GetActorParam(pH);
    C_EquipmentManager* em = ResolveEquipment(actor);
    if (!em)
        return pH->EndFunction();
    Offsets::IScriptTable* t = m_pSS->CreateTable(0, 0);
    if (!t)
        return pH->EndFunction();
    t->SetValueAny("total", ScriptAnyValue(em->m_totalWeight));
    t->SetValueAny("worn", ScriptAnyValue(em->m_wornWeight));
    int n = pH->EndFunctionAny(ScriptAnyValue(t));
    t->Release();
    return n;
}

int CScriptBind_EquipmentManager::GetInventoryEx(Offsets::IFunctionHandler* pH)
{
    C_Actor* actor = GetActorParam(pH);
    C_Inventory* inv = ResolveInventory(actor);
    if (!inv)
        return pH->EndFunction();
    Offsets::IScriptTable* t = m_pSS->CreateTable(0, 0);
    if (!t)
        return pH->EndFunction();
    int idx = 1;
    for (C_Item* item : inv->m_items) {
        if (!item)
            continue;
        if (Offsets::IScriptTable* it = BuildItemTable(m_pSS, item)) {
            t->SetAtAny(idx++, ScriptAnyValue(it));
            it->Release();   // parent table's Lua ref keeps it alive
        }
    }
    int n = pH->EndFunctionAny(ScriptAnyValue(t));
    t->Release();
    return n;
}

int CScriptBind_EquipmentManager::GetInventoryId(Offsets::IFunctionHandler* pH)
{
    C_Actor* actor = GetActorParam(pH);
    C_Inventory* inv = ResolveInventory(actor);
    if (!inv)
        return pH->EndFunction();
    return pH->EndFunctionAny(HandleValue(inv->m_wuid.m_value));
}

// ---- mutator --------------------------------------------------------------

int CScriptBind_EquipmentManager::SetItemEquipped(Offsets::IFunctionHandler* pH)
{
    C_Actor* actor = GetActorParam(pH);
    auto* invSoul = ResolveInventorySoul(actor);
    uint64_t itemId = 0;
    bool equip = true;
    if (!invSoul || !GetHandleParam(pH, 2, itemId) || !pH->GetParam(3, equip))
        return pH->EndFunction();
    C_Item* item = ResolveItem(itemId);
    if (!item)
        return pH->EndFunction();
    if (equip)
        invSoul->EquipItem(item, true);
    else
        invSoul->UnequipItem(item, true);
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

}  // namespace luautils
