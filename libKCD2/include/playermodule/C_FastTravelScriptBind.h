#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_FastTravelScriptBind -- Lua "FastTravel" table (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_FastTravelScriptBind@playermodule@wh@@ (TD 0x184DFA0B8, COL 0x1841ACB8)
// vtable 0x183ACEE00  ctor sub_180EF76A0 (registers inline; helper sub_180ECB65C holds the name).
// alloc 96 (0x60 == sizeof, PROVEN: script-bind bootstrap sub_180EF7184 @0x180EF726C) -- adds NO
// data over CScriptableBase; owned at bind-holder(+C_PlayerModule+0x18)+0x78.
// [FUNDAMENTAL vs KCD1] KCD1's DespawnLastRandomEvent / DespawnRandomEvent / StopToEvent are GONE
// (random encounters moved to the standalone C_RandomEventManager); only Stop survives.
//
// -- Lua methods (name(argSig) -> handler) --
//   Stop()   0x182ED03BC

namespace wh::playermodule {

class C_FastTravelScriptBind : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FastTravelScriptBind;
};
static_assert(sizeof(C_FastTravelScriptBind) == 0x60, "C_FastTravelScriptBind must be 0x60");

}  // namespace wh::playermodule
