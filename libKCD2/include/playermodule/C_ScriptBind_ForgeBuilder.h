#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBind_ForgeBuilder -- Lua "ForgeBuilder" table (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// NEW in KCD2 (no KCD1 counterpart). RTTI .?AVC_ScriptBind_ForgeBuilder@playermodule@wh@@
// (TD 0x184E11F70, COL 0x4469958)  vtable 0x183F7D480  ctor sub_182040F70 (registers inline;
// helper sub_18203FDF0 holds the shared argSig).
// alloc 104 (0x68 == sizeof, bootstrap sub_180EF7184 @0x180EF730A); owner @+0x60.
//
// -- Lua methods (name(argSig) -> handler) --
//   StartMinigame(userId, forgeBuilderTriggeId)   0x182042100   [sic -- "TriggeId" typo is
//   CanUse(userId, forgeBuilderTriggeId)          0x182041500    verbatim from the binary]

namespace wh::playermodule {

class C_ScriptBind_ForgeBuilder : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBind_ForgeBuilder;
    void* m_pOwner;   // +0x60  ctor sub_182040F70 stores a2 = owning bind-holder (C_PlayerModule::m_sub18,
                      //        0xB0 aggregate at C_PlayerModule+0x18, ctor sub_180EF7184). No RTTI on holder
                      //        so void* is retained -- NOT C_PlayerModule* (that is 0x18 higher).
};
static_assert(sizeof(C_ScriptBind_ForgeBuilder) == 0x68, "C_ScriptBind_ForgeBuilder must be 0x68");

}  // namespace wh::playermodule
