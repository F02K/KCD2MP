#include "scriptbind/ScriptBind_ItemManager.h"

#include <algorithm>

#include "LuaHelpers.h"
#include "ResolveHelpers.h"

#include "Offsets/vtables/IFunctionHandler.h"
#include "Offsets/vtables/IScriptSystem.h"
#include "Offsets/vtables/IScriptTable.h"
#include "entitymodule/C_EquippableItemRuntimeData.h"
#include "entitymodule/C_Inventory.h"
#include "entitymodule/C_InventoryManager.h"
#include "entitymodule/C_Item.h"
#include "entitymodule/E_ItemType.h"
#include "framework/WUID.h"

using namespace wh::entitymodule;
using wh::framework::WUID;

namespace luautils {

void CScriptBind_ItemManagerExt::Attach(Offsets::IScriptSystem* pSS, Offsets::IScriptTable* pTable)
{
    if (!pSS || !pTable)
        return;
    m_pSS = pSS;
    m_pTable = pTable;

    RegisterFunction("GetItemEx", "itemId", functor(*this, &CScriptBind_ItemManagerExt::GetItemEx));
    RegisterFunction("IsItemEquipped", "itemId", functor(*this, &CScriptBind_ItemManagerExt::IsItemEquipped));
    RegisterFunction("GetItemCondition", "itemId", functor(*this, &CScriptBind_ItemManagerExt::GetItemCondition));
    RegisterFunction("GetItemQuality", "itemId", functor(*this, &CScriptBind_ItemManagerExt::GetItemQuality));
    RegisterFunction("GetItemMaxQuality", "itemId", functor(*this, &CScriptBind_ItemManagerExt::GetItemMaxQuality));
    RegisterFunction("GetItemPrices", "itemId", functor(*this, &CScriptBind_ItemManagerExt::GetItemPrices));
    RegisterFunction("SetItemHealth", "itemId, health", functor(*this, &CScriptBind_ItemManagerExt::SetItemHealth));
    RegisterFunction("SetItemCondition", "itemId, condition", functor(*this, &CScriptBind_ItemManagerExt::SetItemCondition));
    RegisterFunction("SetItemQuality", "itemId, quality", functor(*this, &CScriptBind_ItemManagerExt::SetItemQuality));
    RegisterFunction("SetItemAmount", "itemId, amount", functor(*this, &CScriptBind_ItemManagerExt::SetItemAmount));
    RegisterFunction("SetItemOwner", "itemId, ownerId, [stolenFromOwnerId]", functor(*this, &CScriptBind_ItemManagerExt::SetItemOwner));
    RegisterFunction("WashItem", "itemId, [maxEffect]", functor(*this, &CScriptBind_ItemManagerExt::WashItem));
    RegisterFunction("SetItemPhaseId", "itemId, phaseId", functor(*this, &CScriptBind_ItemManagerExt::SetItemPhaseId));
    RegisterFunction("SetItemPhase", "itemId, phase", functor(*this, &CScriptBind_ItemManagerExt::SetItemPhase));
    RegisterFunction("AdvanceItemPhase", "itemId, amount", functor(*this, &CScriptBind_ItemManagerExt::AdvanceItemPhase));
    RegisterFunction("MoveItem", "itemId, dstInventoryId, [count]", functor(*this, &CScriptBind_ItemManagerExt::MoveItem));
}

void CScriptBind_ItemManagerExt::RegisterFunction(const char* sName, const char* sParams, const FunctionFunctor& f)
{
    SUserFunctionDesc fd;
    fd.sGlobalName     = "ItemManager";
    fd.sFunctionName   = sName;
    fd.sFunctionParams = sParams;
    fd.pFunctor        = f;
    m_pTable->AddFunction(fd);
}

// ---- shared param helpers -------------------------------------------------

static C_Item* GetItemParam(Offsets::IFunctionHandler* pH)
{
    uint64_t id = 0;
    if (!GetHandleParam(pH, 1, id))
        return nullptr;
    return ResolveItem(id);
}

// ---- queries --------------------------------------------------------------

int CScriptBind_ItemManagerExt::GetItemEx(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    if (!item)
        return pH->EndFunction();
    Offsets::IScriptTable* t = BuildItemTable(m_pSS, item);
    if (!t)
        return pH->EndFunction();
    int n = pH->EndFunctionAny(ScriptAnyValue(t));
    t->Release();
    return n;
}

int CScriptBind_ItemManagerExt::IsItemEquipped(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    if (!item)
        return pH->EndFunction();
    return pH->EndFunctionAny(ScriptAnyValue((item->m_flags & 1) != 0));
}

int CScriptBind_ItemManagerExt::GetItemCondition(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    if (!item)
        return pH->EndFunction();
    return pH->EndFunctionAny(ScriptAnyValue(item->GetCondition()));
}

int CScriptBind_ItemManagerExt::GetItemQuality(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    if (!item)
        return pH->EndFunction();
    return pH->EndFunctionAny(ScriptAnyValue(item->GetQuality()));
}

int CScriptBind_ItemManagerExt::GetItemMaxQuality(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    if (!item)
        return pH->EndFunction();
    return pH->EndFunctionAny(ScriptAnyValue(item->GetMaxQuality()));
}

int CScriptBind_ItemManagerExt::GetItemPrices(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    if (!item)
        return pH->EndFunction();
    Offsets::IScriptTable* t = m_pSS->CreateTable(0, 0);
    if (!t)
        return pH->EndFunction();
    t->SetValueAny("unit", ScriptAnyValue(item->GetCurrentUnitPrice()));
    t->SetValueAny("stack", ScriptAnyValue(item->GetCurrentStackPrice()));
    t->SetValueAny("newUnit", ScriptAnyValue(item->GetNewUnitPrice()));
    t->SetValueAny("newStack", ScriptAnyValue(item->GetNewStackPrice()));
    int n = pH->EndFunctionAny(ScriptAnyValue(t));
    t->Release();
    return n;
}

// ---- setters --------------------------------------------------------------

int CScriptBind_ItemManagerExt::SetItemHealth(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    float health = 1.0f;
    if (!item || !pH->GetParam(2, health))
        return pH->EndFunction();
    item->SetItemHealth(health);
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::SetItemCondition(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    float condition = 1.0f;
    if (!item || !pH->GetParam(2, condition))
        return pH->EndFunction();
    condition = std::clamp(condition, 0.0f, 1.0f);
    if (item->IsOfType(E_ItemType::Equippable)) {
        auto* rt = static_cast<C_EquippableItemRuntimeData*>(item->GetOrCreateRuntimeData());
        if (!rt)
            return pH->EndFunction();
        rt->m_condition = condition;
        item->NotifyChanged(0);
    } else {
        // Non-equippables have no runtime-data condition; +0x54 IS the condition.
        item->SetHealth(condition);
    }
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::SetItemQuality(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    int quality = 0;
    if (!item || !pH->GetParam(2, quality))
        return pH->EndFunction();
    if (!item->IsOfType(E_ItemType::Equippable))
        return pH->EndFunction();
    auto* rt = static_cast<C_EquippableItemRuntimeData*>(item->GetOrCreateRuntimeData());
    if (!rt)
        return pH->EndFunction();
    rt->m_quality = quality;
    item->NotifyChanged(0);
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::SetItemAmount(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    int amount = 0;
    if (!item || !pH->GetParam(2, amount))
        return pH->EndFunction();
    C_InventoryBase* inv = item->m_pInventory;
    if (amount <= 0) {
        if (!inv)
            return pH->EndFunction();
        inv->RemoveItem(item, 2, (uint32_t)item->m_amount);   // reason 2 = script delete
        return pH->EndFunctionAny(ScriptAnyValue(true));
    }
    int delta = amount - item->m_amount;
    if (delta == 0)
        return pH->EndFunctionAny(ScriptAnyValue(true));
    if (inv) {
        if (!inv->ChangeItemAmount(item, delta))
            return pH->EndFunction();
    } else {
        item->SetAmount(amount);   // un-parented item: raw write, no listeners to fire
    }
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::SetItemOwner(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    uint64_t owner = 0;
    if (!item || !GetHandleParam(pH, 2, owner))
        return pH->EndFunction();
    uint64_t stolenFromOwner = owner;
    if (pH->GetParamCount() >= 3 && !GetHandleParam(pH, 3, stolenFromOwner))
        return pH->EndFunction();
    item->SetOwner(WUID{ owner }, WUID{ stolenFromOwner }, true);
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::WashItem(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    if (!item)
        return pH->EndFunction();
    float maxEffect = 1.0f;
    if (pH->GetParamCount() >= 2 && !pH->GetParam(2, maxEffect))
        return pH->EndFunction();
    item->WashItem(maxEffect);
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::SetItemPhaseId(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    int phaseId = 0;
    if (!item || !pH->GetParam(2, phaseId))
        return pH->EndFunction();
    item->SetItemPhaseId((uint32_t)phaseId);
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::SetItemPhase(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    float phase = 0.0f;
    if (!item || !pH->GetParam(2, phase))
        return pH->EndFunction();
    item->SetItemPhase(phase);
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::AdvanceItemPhase(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    float amount = 0.0f;
    if (!item || !pH->GetParam(2, amount))
        return pH->EndFunction();
    item->AdvanceItemPhase(amount);
    return pH->EndFunctionAny(ScriptAnyValue(true));
}

int CScriptBind_ItemManagerExt::MoveItem(Offsets::IFunctionHandler* pH)
{
    C_Item* item = GetItemParam(pH);
    uint64_t dstId = 0;
    if (!item || !GetHandleParam(pH, 2, dstId))
        return pH->EndFunction();
    int count = 0;   // 0 = whole stack
    if (pH->GetParamCount() >= 3 && !pH->GetParam(3, count))
        return pH->EndFunction();
    auto* invMgr = C_InventoryManager::GetInstance();
    if (!invMgr)
        return pH->EndFunction();
    C_Inventory* dst = invMgr->LookupByWUID(WUID{ dstId });
    if (!dst)
        return pH->EndFunction();
    C_Item* moved = dst->MoveItemIn(item, (uint32_t)count, false);
    if (!moved)
        return pH->EndFunction();
    return pH->EndFunctionAny(HandleValue(moved->m_wuid.m_value));
}

}  // namespace luautils
