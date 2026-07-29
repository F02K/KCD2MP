#pragma once
#include "crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::guimodule::C_ScriptBindMap -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// RTTI .?AVC_ScriptBindMap@guimodule@wh@@ (TD 0x184C92468), vtable 0x183B8EFB8.
// ctor sub_18146C6FC. Lua global table "UIMap".
//
// [MODERATE vs KCD1] Same class/global name and 0x70 size as KCD1, but:
//   * no longer heap-allocated -- EMBEDDED in C_UIMap at +0x6C8 (C_UIMap ctor
//     sub_18146C3C4 @0x18146c5f9; next member at +0x738 pins the 0x70 extent);
//   * the KCD1 function UIMap.SetPlaceDiscovered was DROPPED. Only one function
//     survives.
//
// Registered script functions (ctor tail, name-descriptor helper sub_1819C63F8 ->
// m_pMethodsTable vf[+0xB0]):
//   UIMap.GoToCheckpointMark  -> 0x182BA7810   (KCD1: 0x1806A71FC)

namespace wh::guimodule {

class C_ScriptBindMap : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindMap;
    // Same bind tail pair as C_ScriptBindUI (see there; +0x60 identity UNVERIFIED).
    Offsets::ISystem*       m_pSystem;          // +0x60  gEnv ISystem: framework->GetISystem() (IGameFramework vf[0x98]); its vf[0x278]() = gEnv->pScriptSystem -> m_pScriptSystem@+0x68
    Offsets::IScriptSystem* m_pScriptSystem;    // +0x68
};
static_assert(sizeof(C_ScriptBindMap) == 0x70, "C_ScriptBindMap must be 0x70");

}  // namespace wh::guimodule
