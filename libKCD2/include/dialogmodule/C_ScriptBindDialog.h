#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::dialogmodule::C_ScriptBindDialog -- Lua "DialogModule" table (KCD2 1.5.6, kd7u).
// sizeof 0x78 (alloc 120 in the C_DialogManager ctor; stored at manager +0x128).
// -----------------------------------------------
// RTTI TD 0x184B53230; vtable 0x183AB8058 (4 slots) : ::CScriptableBase (0x60). ctor
// sub_180E30ACC(this, mgr): grabs the script-system service
// (S_GameContext->(+8)->vf[19]()) into +0x60, its Lua-state/methods object
// (svc->vf[79]()) into +0x68, Init-scriptable sub_18144CE0C, global table name
// "DialogModule" via sub_180B85140, then sub_1818420E8 registers the function set.
// (CScriptableBase's own m_pMethodsTable/m_pSS live INSIDE the 0x60 base; the +0x60/
// +0x68 derived slots are the Warhorse service/Lua-state pair -- pointee types
// unresolved, modeled void*.)
//
// Lua fn table (registered sub_1818420E8):
//   ForceDialog(speakerId, listenerId)    0x18280FB54   <-- CHEAT: start any conversation
//   SetAIInteractionState(soulId)         0x18280FC18
//   IsSoulInDialog(soulId)                0x18280FBBC
//   SetPlayerInteractiveState(stateName)  0x1804CFA8C
//   AnalyzeRequest(...)                   0x18280F9A0
//   ResetHaggle(...)                      0x1819C6128   <-- CHEAT: reset trade minigame

namespace wh::dialogmodule {

class C_DialogManager;

class C_ScriptBindDialog : public ::CScriptableBase   // +0x00  (0x60; own +0x08 byte set 0 by ctor)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindDialog;
    Offsets::ISystem*       m_pScriptSvc;   // +0x60  ISystem (gameCtx->(+8)->vf[19](); passed to CScriptableBase::Init as pSystem)
    Offsets::IScriptSystem* m_pLuaState;    // +0x68  IScriptSystem = ISystem->vf[79] (GetIScriptSystem); same value stored at base m_pSS (+0x50)
    C_DialogManager* m_pOwner;       // +0x70  owning C_DialogManager (ctor arg2)
};
static_assert(sizeof(C_ScriptBindDialog) == 0x78, "C_ScriptBindDialog must be 0x78 (alloc 120)");

}  // namespace wh::dialogmodule
