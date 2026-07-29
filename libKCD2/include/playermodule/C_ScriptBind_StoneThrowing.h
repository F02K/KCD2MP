#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBind_StoneThrowing -- Lua "StoneThrowing" table (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// NEW in KCD2 (no KCD1 counterpart). RTTI .?AVC_ScriptBind_StoneThrowing@playermodule@wh@@
// (TD 0x184E12120, COL 0x4118990)  vtable 0x183ACF1C0  ctor sub_180EF789C (registers inline;
// helper sub_180EF7AD0 holds the shared argSig -- string not extracted).
// alloc 104 (0x68 == sizeof, bootstrap sub_180EF7184 @0x180EF728A); owner @+0x60.
//
// -- Lua methods (name -> handler) --
//   StartMinigame(...)   0x182ED0374
//   CanUse(...)          0x182ECCAD4

namespace wh::playermodule {

class C_ScriptBind_StoneThrowing : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBind_StoneThrowing;
    void* m_pOwner;   // +0x60  = C_PlayerModule::m_sub18 (+0x18) bind holder (0xB0, ctor sub_180EF7184); ctor 0x180EF78A8 stores a2; holder has no RTTI/vtable -> concrete class unnamed
};
static_assert(sizeof(C_ScriptBind_StoneThrowing) == 0x68, "C_ScriptBind_StoneThrowing must be 0x68");

}  // namespace wh::playermodule
