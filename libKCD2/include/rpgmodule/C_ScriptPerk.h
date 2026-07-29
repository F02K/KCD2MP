#pragma once
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_ScriptPerk -- script-driven perk (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_ScriptPerk@rpgmodule@wh@@ (TD 0x184D53E80)  vtable 0x183B7D5C0.
// sizeof 0x30 (PROVEN: factory sub_1813F6648 operator new(48)) -- no members beyond C_Perk.
// (An S_ScriptPerkId value type also exists in RTTR -- its relation to m_id is not traced.)

namespace wh::rpgmodule {

class C_ScriptPerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptPerk;
};
static_assert(sizeof(C_ScriptPerk) == 0x30, "C_ScriptPerk must be 0x30 (operator new(48))");

}  // namespace wh::rpgmodule
