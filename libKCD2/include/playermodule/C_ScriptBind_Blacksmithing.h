#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBind_Blacksmithing -- Lua "Blacksmithing" table (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// NEW in KCD2 (no KCD1 counterpart). RTTI .?AVC_ScriptBind_Blacksmithing@playermodule@wh@@
// (TD 0x184E12170, COL 0x4118878)  vtable 0x183ACF1E8  ctor sub_180EF77D0 (registers inline;
// helper sub_180EF7968 holds the shared argSig).
// alloc 104 (0x68 == sizeof, bootstrap sub_180EF7184 @0x180EF72AB); owner @+0x60.
//
// -- Lua methods (name(argSig) -> handler) --
//   StartMinigame(userId, smitheryId)   0x182ED032C
//   CanUse(userId, smitheryId)          0x180BE1838

namespace wh::playermodule {

class C_ScriptBind_Blacksmithing : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBind_Blacksmithing;
    void* m_pOwner;   // +0x60  ctor sub_180EF77D0 stores a2 = owning bind-holder (C_PlayerModule::m_sub18,
                      //        0xB0 aggregate at C_PlayerModule+0x18, ctor sub_180EF7184). No RTTI on holder
                      //        so void* is retained -- NOT C_PlayerModule* (that is 0x18 higher).
};
static_assert(sizeof(C_ScriptBind_Blacksmithing) == 0x68, "C_ScriptBind_Blacksmithing must be 0x68");

}  // namespace wh::playermodule
