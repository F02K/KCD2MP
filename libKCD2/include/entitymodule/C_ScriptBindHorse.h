#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"
#include "../crysystem/CScriptTable.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindHorse -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78 (CONFIRMED: alloc 120).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindHorse@entitymodule@wh@@ (TD 0x184C75858, COL 0x1843316D8; CHD = [self,
// CScriptableBase])  vftable 0x183ECB330  ctor sub_18128C614  dtor sub_182AB72B4  factory sub_180E30204.
//
// NO SetGlobalName: per-entity bind; methods attach to horse entity script tables (access path
// inferred from the entity-bind pattern). Handlers are pointer-to-virtual-member thunks resolved
// against THIS vtable.
//
// Lua fn table (3 fns; registered in ctor):
//   HasRider          0x1814DB218  vtable+0x28 (s5)  ()
//   IsMountable       0x1814DB1A4  vtable+0x30 (s6)  ()
//   RearAndThrowDown  0x182AC0CC4  vtable+0x38 (s7)  ()

namespace wh::entitymodule {

class C_ScriptBindHorse : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindHorse;
    ::CScriptTable* m_pParamTable;   // +0x60  obj created via sub_180AED4A8, released via sub_1804FB980 (INFERRED SmartScriptTable)
    Offsets::ISystem* m_pContext;    // +0x68  ctor a2 = ISystem*; a2->vf[0x278] returns gEnv->pScriptSystem, fed to CScriptableBase::Init
    void*           m_pManager;      // +0x70  = ISystem::vf[0x2C8](slot89 -> gEnv->pGame/IGame*)->IGame::vf16() = *(whGlobal sub_1809155C8()+8); same engine-root obj as C_ScriptBindActor::m_pSubsystem
};
static_assert(sizeof(C_ScriptBindHorse) == 0x78, "C_ScriptBindHorse must be 0x78");

}  // namespace wh::entitymodule
