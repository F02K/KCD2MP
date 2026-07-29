// LuaUtils global `EquipmentManager` Lua table (new global, MCM SetGlobalAny
// pattern). Entity-keyed equip queries the vanilla surface never exposed.
// Dot-call only; ids are ScriptHandles (see LuaHelpers.h).
//
//   EquipmentManager.GetEquippedItems(entityId)       -> { itemId, ... } | nil
//   EquipmentManager.GetEquippedClothing(entityId)    -> { [equipmentSlotId] = itemId } | nil
//   EquipmentManager.GetHandSlots(entityId)           -> { [1..8] = itemId } | nil  (empty slots absent)
//   EquipmentManager.GetItemInSlot(entityId, slotId)  -> itemId | nil   (clothing slots only)
//   EquipmentManager.GetEquipWeights(entityId)        -> { total, worn } | nil
//   EquipmentManager.GetInventoryEx(entityId)         -> { itemTable, ... } | nil  (GetItemEx shape)
//   EquipmentManager.GetInventoryId(entityId)         -> inventoryId | nil
//   EquipmentManager.SetItemEquipped(entityId, itemId, equip) -> true | nil
//
// GetHandSlots maps Lua 1..8 to E_WeaponEquipSlot values 0..7:
// primary main/off, secondary main/off, oversized/oversized-off, torch, dagger.
// It does not expose player outfit QAM assignments. Equipped belt and pouch
// container items are data-driven clothing slots 44 and 45, respectively.
//
// Caveat: distant/unstreamed NPCs may not have worn items materialized as real
// C_Items yet, and some souls carry no equipment manager. Every function is
// nil-graceful rather than erroring.

#pragma once

#include "crysystem/SUserFunctionDesc.h"

namespace Offsets {
struct IFunctionHandler;
struct IScriptSystem;
struct IScriptTable;
}

namespace luautils {

class CScriptBind_EquipmentManager
{
public:
    // CreateTable + AddRef (pinned for the session) + register + SetGlobalAny.
    void Init(Offsets::IScriptSystem* pSS);
    bool IsInitialized() const { return m_pMethodsTable != nullptr; }

    int GetEquippedItems(Offsets::IFunctionHandler* pH);
    int GetEquippedClothing(Offsets::IFunctionHandler* pH);
    int GetHandSlots(Offsets::IFunctionHandler* pH);
    int GetItemInSlot(Offsets::IFunctionHandler* pH);
    int GetEquipWeights(Offsets::IFunctionHandler* pH);
    int GetInventoryEx(Offsets::IFunctionHandler* pH);
    int GetInventoryId(Offsets::IFunctionHandler* pH);
    int SetItemEquipped(Offsets::IFunctionHandler* pH);

private:
    void RegisterFunction(const char* sName, const char* sParams, const FunctionFunctor& f);

    Offsets::IScriptSystem* m_pSS = nullptr;
    Offsets::IScriptTable*  m_pMethodsTable = nullptr;
};

inline CScriptBind_EquipmentManager g_equipmentManagerBind;

}  // namespace luautils
