#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::rpgmodule::C_LocationScriptBind -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60.
// -----------------------------------------------
// RTTI .?AVC_LocationScriptBind@rpgmodule@wh@@ (CHD = [self, CScriptableBase], mdisp 0)
// vftable 0x183B55288.  Adds NO members beyond CScriptableBase (ctor only writes base fields;
// Init flag = 1, instance methods). Embedded BY VALUE in C_ScriptBindRPGModule @+0xD0; its
// methods table is the metatable for location USERDATA objects (RPG.GetLocationById(...) results).
//
// Lua fn table (10 fns; registrar sub_181675750):
//   __tostring     0x182CFFBFC  ()
//   GetId          0x182CF4CB0  ()
//   GetName        0x182CF5FC8  ()
//   GetCategory    0x182CF4400  ()
//   GetState       0x182CF7594  ()
//   SetState       0x18066CD10  (state)
//   IsDiscovered   0x182CF8100  ()
//   GetPosition    0x182CF6918  ()
//   GetReputation  0x182CF6DD0  ()
//   IsWanted       0x182CF8884  ()

namespace wh::rpgmodule {

class C_LocationScriptBind : public ::CScriptableBase   // +0x00  (0x60; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LocationScriptBind;
};
static_assert(sizeof(C_LocationScriptBind) == 0x60, "C_LocationScriptBind must be 0x60");

}  // namespace wh::rpgmodule
