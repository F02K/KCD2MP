#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindItemManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x68 (CONFIRMED: alloc 104).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindItemManager@entitymodule@wh@@ (TD 0x184C75A88, COL 0x18413E050; CHD = [self,
// CScriptableBase])  vftable 0x183AB7F78 (4 slots; [0] dtor sub_1825CDCFC)  ctor inlined in factory
// sub_180E30204 @~0x180E3052E.
//
// GLOBAL Lua table "ItemManager" (SetGlobalName sub_180B85140 -> SetGlobalValue;
// m_bInstanceMethods=0, PROVEN). Handlers are DIRECT member-fn delegates.
//
// Lua fn table `ItemManager` (7 fns; registrar sub_1812B2D38 = REL::ID(104708) --
// single arg = this; Projects/LuaUtils hooks it to append its extension fns into
// m_pMethodsTable after the vanilla registration):
//   RemoveItem       0x182AC0E54  (itemId)
//   GetItem          0x182ABC0BC  (itemId)
//   GetItemUIName    0x182ABC354  (classId)
//   GetItemName      0x182ABC25C  (classId)
//   GetItemOwner     0x182ABC2F4  (id)
//   IsItemOversized  0x182ABE3FC  (id)
//   AddOnEquipBuff   0x182AB77BC  (itemId, buff_id, add)   CHEAT-adjacent (buff on item equip)

namespace wh::entitymodule {

class C_ScriptBindItemManager : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindItemManager;
    void* m_pSystem;   // +0x60  = *(scriptbind-collection + 0xC8) (factory 0x180E3055A reads [rsi+0xC8], 0x180E30577 stores [r15+60h]); NOT ctx; pointee UNRESOLVED
};
static_assert(sizeof(C_ScriptBindItemManager) == 0x68, "C_ScriptBindItemManager must be 0x68");

}  // namespace wh::entitymodule
