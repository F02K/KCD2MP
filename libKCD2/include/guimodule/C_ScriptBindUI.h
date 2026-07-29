#pragma once
#include "crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::guimodule::C_ScriptBindUI -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// RTTI .?AVC_ScriptBindUI@guimodule@wh@@ (TD 0x184CD1078), vtable 0x183C12460
// (4 slots, CScriptableBase shape). Lua global table "UI".
//
// [FUNDAMENTAL vs KCD1] NEW class (no KCD1 counterpart). Not heap-allocated: lives
// EMBEDDED inside C_GUIModule at +0x98 and is initialized inline by the module ctor
// sub_181789268 (@0x1817892b6..0x181789350): vtable install, CScriptableBase::Init
// (sub_18144CE0C), SetGlobalName("UI") (sub_180B85140), then the registration body
// sub_180B631CC.
//
// Registered script functions (sub_180B631CC; descriptor invoker + delegate handler):
//   UI.OnApseLinkNodeSpawn            -> 0x180ED52E4
//   UI.OnApseLinkNodeDespawn          -> 0x180ED6540
//   UI.OnShopLinkNodeSpawn            -> 0x180ED517C
//   UI.OnShopLinkNodeDespawn          -> 0x180ED40C4
//   UI.OnForgeBuilderLinkNodeSpawn    -> 0x181F8CC90
//   UI.OnForgeBuilderLinkNodeDespawn  -> 0x181F8CC50
// (Lua -> C++ notifications that a level "link node" entity for the Apse character
// screen / shop / forge-builder 3D scenes spawned or despawned.)

namespace wh::guimodule {

class C_ScriptBindUI : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindUI;
    // +0x60/+0x68: same tail pair as every wh KCD2 bind -- ctor stores
    // (*(gameGetter sub_1809155C8() + 8))->vf[+0x98]() at +0x60 and that object's
    // vf[+0x278]() (the Init a2 / script-system accessor) at +0x68. Names follow the
    // KCD1 C_ScriptBindMap precedent (identity of +0x60 UNVERIFIED beyond "framework
    // object the script system hangs off").
    Offsets::ISystem*       m_pSystem;          // +0x60  gEnv ISystem: framework->GetISystem() (IGameFramework vf[0x98]); its vf[0x278]() = gEnv->pScriptSystem -> m_pScriptSystem@+0x68
    Offsets::IScriptSystem* m_pScriptSystem;    // +0x68
};
static_assert(sizeof(C_ScriptBindUI) == 0x70, "C_ScriptBindUI must be 0x70");

}  // namespace wh::guimodule
