#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBind_Alchemy -- Lua "Alchemy" table (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_ScriptBind_Alchemy@playermodule@wh@@ (TD 0x184E12410, COL 0x4118850)
// vtable 0x183ACEC68  ctor sub_180EF737C  registration sub_1817C0E7C.
// alloc 104 (0x68 == sizeof, bootstrap sub_180EF7184 @0x180EF71E6); owner @+0x60.
//
// -- Lua methods (name(argSig) -> handler) --
//   StartAlchemy(userId, tableId)   0x180896C64
//   ResetTable(tableId)             0x182ECF2A8
//   GetUsableFor(userId, item_type)         0x1811EDAC8
//   IsUsableEnabledFor(userId, item_type)   0x1808D1F2C
//   IsUsableVisibleFor(userId, item_type)   0x1808D1DE0
//   IsUsableByHold(userId, item_type)       0x1808D1E40
//   GetDisableReason(userId, item_type)     0x1804CF0CC
//   GetFlagTextFor(userId, item_type)       0x1804CEEF8
//   IsCrossCenteringEnabled(userId, item_type)  0x18149E06C
//   OnUse(userId)                   0x180D5AE04
// (the 6 usable-query argSigs come from the shared helper sub_1817C0FFC)

namespace wh::playermodule {

class C_ScriptBind_Alchemy : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBind_Alchemy;
    void* m_pOwner;   // +0x60  ctor sub_180EF737C stores a2 = owning bind-holder (C_PlayerModule::m_sub18,
                      //        0xB0 aggregate at C_PlayerModule+0x18, ctor sub_180EF7184). No RTTI on holder
                      //        so void* is retained -- NOT C_PlayerModule* (that is 0x18 higher).
};
static_assert(sizeof(C_ScriptBind_Alchemy) == 0x68, "C_ScriptBind_Alchemy must be 0x68");

}  // namespace wh::playermodule
