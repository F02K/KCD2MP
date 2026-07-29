#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::rpgmodule::C_FactionScriptBind -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60.
// -----------------------------------------------
// RTTI .?AVC_FactionScriptBind@rpgmodule@wh@@ (CHD = [self, CScriptableBase], mdisp 0)
// vftable 0x183B55260.  Adds NO members beyond CScriptableBase (ctor only writes base fields;
// Init flag = 1, instance methods). Embedded BY VALUE in C_ScriptBindRPGModule @+0x70; its
// methods table is the metatable for faction USERDATA objects (RPG.GetFactionById(...) results).
//
// Lua fn table (7 fns; registrar sub_1812E206C):
//   __tostring         0x182CFFB2C  ()
//   GetId              0x182CF4C6C  ()
//   GetName            0x182CF5E3C  ()
//   GetLocationId      0x182CF5430  ()
//   GetReputation      0x182CF6D3C  ()
//   GetBaseReputation  0x182CF3C20  ()
//   AddReputation      0x182CEE27C  (sEnumName)   -- reputation_change entry name

namespace wh::rpgmodule {

class C_FactionScriptBind : public ::CScriptableBase   // +0x00  (0x60; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionScriptBind;
};
static_assert(sizeof(C_FactionScriptBind) == 0x60, "C_FactionScriptBind must be 0x60");

}  // namespace wh::rpgmodule
