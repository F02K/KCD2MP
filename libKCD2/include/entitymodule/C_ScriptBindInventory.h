#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindInventory -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70 (CONFIRMED: alloc 112).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindInventory@entitymodule@wh@@ (TD 0x184C75900, COL 0x18413E028; CHD = [self,
// CScriptableBase])  vftable 0x183AB7FA0 (5 slots; [0] dtor sub_18260F330, [4] sub_1824AA0F8
// returns sizeof=112)  ctor inlined in factory sub_180E30204 @~0x180E303F7.
//
// NO SetGlobalName (name buffer zeroed; m_bInstanceMethods=1, PROVEN from Init r9d): INSTANCED
// per-entity methods table -- the entity's inventory table in Lua (e.g. <entity>.inventory:AddItem,
// matching KCD1; exact attach mechanism inferred). Handlers here are DIRECT member-fn delegates.
//
// Lua fn table (16 fns; registrar sub_1812B2F0C):
//   AddItem             0x182AB76B4  (item)                          -- ScriptHandle/EntityId
//   DeleteItem          0x182AB8C70  (id, count)
//   DeleteItemOfClass   0x182AB8C78  (guid, count)
//   MoveItemOfClass     0x182ABF1C8  (inv guid, count, changeOwner)
//   RemoveAllItems      0x182AC0D88  ()
//   Dump                0x182AB9D90  ()
//   GetCount            0x182ABB49C  ()
//   GetCountOfCategory  0x182ABB4E8  (categoryName)
//   GetCountOfClass     0x182ABB538  (classId)
//   GetId               0x182ABBEE8  ()
//   HasItem             0x182ABDCF0  (itemId)
//   FindItem            0x182ABA378  (classId)
//   RemoveMoney         0x182AC0F90  (count)                         CHEAT-adjacent
//   GetMoney            0x1819C6B58  ()                              -- reads *(entity+232)*0.1
//   CreateItem          0x182AB8884  (classId, health, amount)       *** CHEAT: spawn items ***
//   GetInventoryTable   0x182ABBFF8  ()

namespace wh::entitymodule {

class C_ScriptBindInventory : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindInventory;
    void* m_pSystem;     // +0x60  = ctx (factory 0x180E30439 stores r14 = same framework obj as base Init a3; (S_GameContext+8)->vf[19]())
    void* m_pGlobal68;   // +0x68  = qword_18492D8A0 (runtime-init global, 0 in image; same global as rpgmodule C_ScriptBindSoul m_pGlobalSys); type UNRESOLVED
};
static_assert(sizeof(C_ScriptBindInventory) == 0x70, "C_ScriptBindInventory must be 0x70");

}  // namespace wh::entitymodule
