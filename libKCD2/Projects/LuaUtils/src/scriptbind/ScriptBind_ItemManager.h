// LuaUtils extension of the vanilla global `ItemManager` Lua table.
//
// Not a CScriptableBase: the vanilla wh::entitymodule::C_ScriptBindItemManager
// owns the table; we AddFunction our descriptors into it after its registrar
// runs (hook in plugin.cpp) or, as a fallback, after resolving the global at
// PreDataLoaded. Dot-call only, ids are ScriptHandles (see LuaHelpers.h).
//
//   -- queries
//   ItemManager.GetItemEx(itemId)          -> { id, className, type, amount, health, condition,
//                                              quality, maxQuality, isEquipped, owner, entity } | nil
//   ItemManager.IsItemEquipped(itemId)     -> bool | nil
//   ItemManager.GetItemCondition(itemId)   -> float | nil     (runtime-data aware)
//   ItemManager.GetItemQuality(itemId)     -> int | nil
//   ItemManager.GetItemMaxQuality(itemId)  -> int | nil
//   ItemManager.GetItemPrices(itemId)      -> { unit, stack, newUnit, newStack } | nil
//   -- setters (return true on success, nil otherwise)
//   ItemManager.SetItemHealth(itemId, health)
//   ItemManager.SetItemCondition(itemId, condition)
//   ItemManager.SetItemQuality(itemId, quality)
//   ItemManager.SetItemAmount(itemId, amount)        -- 0 deletes the stack
//   ItemManager.SetItemOwner(itemId, ownerId[, stolenFromOwnerId])
//   ItemManager.WashItem(itemId[, maxEffect])
//   ItemManager.SetItemPhaseId(itemId, phaseId)
//   ItemManager.SetItemPhase(itemId, phase)
//   ItemManager.AdvanceItemPhase(itemId, amount)
//   ItemManager.MoveItem(itemId, dstInventoryId[, count]) -> movedItemId | nil  (count 0/absent = all)

#pragma once

#include "crysystem/SUserFunctionDesc.h"

namespace Offsets {
struct IFunctionHandler;
struct IScriptSystem;
struct IScriptTable;
}

namespace luautils {

class CScriptBind_ItemManagerExt
{
public:
    // Attach to the live ItemManager methods table and register our functions.
    // Re-attaching to the same table re-registers (AddFunction replaces - idempotent).
    void Attach(Offsets::IScriptSystem* pSS, Offsets::IScriptTable* pTable);
    bool IsAttached() const { return m_pTable != nullptr; }

    int GetItemEx(Offsets::IFunctionHandler* pH);
    int IsItemEquipped(Offsets::IFunctionHandler* pH);
    int GetItemCondition(Offsets::IFunctionHandler* pH);
    int GetItemQuality(Offsets::IFunctionHandler* pH);
    int GetItemMaxQuality(Offsets::IFunctionHandler* pH);
    int GetItemPrices(Offsets::IFunctionHandler* pH);
    int SetItemHealth(Offsets::IFunctionHandler* pH);
    int SetItemCondition(Offsets::IFunctionHandler* pH);
    int SetItemQuality(Offsets::IFunctionHandler* pH);
    int SetItemAmount(Offsets::IFunctionHandler* pH);
    int SetItemOwner(Offsets::IFunctionHandler* pH);
    int WashItem(Offsets::IFunctionHandler* pH);
    int SetItemPhaseId(Offsets::IFunctionHandler* pH);
    int SetItemPhase(Offsets::IFunctionHandler* pH);
    int AdvanceItemPhase(Offsets::IFunctionHandler* pH);
    int MoveItem(Offsets::IFunctionHandler* pH);

private:
    void RegisterFunction(const char* sName, const char* sParams, const FunctionFunctor& f);

    Offsets::IScriptSystem* m_pSS = nullptr;
    Offsets::IScriptTable*  m_pTable = nullptr;
};

inline CScriptBind_ItemManagerExt g_itemManagerExt;

}  // namespace luautils
