#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBind_HoleDigging -- Lua "HoleDigging" table (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// NEW in KCD2 (no KCD1 counterpart). RTTI .?AVC_ScriptBind_HoleDigging@playermodule@wh@@
// (TD 0x184E11FE8, COL 0x41188C8)  vtable 0x183ACEE38  ctor sub_180EF7738 (registers inline;
// helper sub_1819C371C holds the name/argSig).
// alloc 96 (0x60 == sizeof, bootstrap sub_180EF7184 @0x180EF72CE) -- adds NO data over
// CScriptableBase (no owner back-pointer, unlike the 0x68 binds).
//
// -- Lua methods (name(argSig) -> handler) --
//   CheckPlayerAboveHole(holeId)   0x182ECCB94

namespace wh::playermodule {

class C_ScriptBind_HoleDigging : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBind_HoleDigging;
};
static_assert(sizeof(C_ScriptBind_HoleDigging) == 0x60, "C_ScriptBind_HoleDigging must be 0x60");

}  // namespace wh::playermodule
