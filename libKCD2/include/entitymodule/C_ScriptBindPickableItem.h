#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindPickableItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88 (CONFIRMED: alloc 136).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindPickableItem@entitymodule@wh@@ (TD 0x184C75940, COL 0x18413E078; CHD = [self,
// CScriptableBase])  vftable 0x183AB7DB0 (4 slots; [0] scalar deleting dtor sub_182AB755C -- body
// sub_182AB731C releases the two SmartScriptTables)  ctor sub_180E2FA24, called from the entitymodule
// bind factory sub_180E30204 @0x180E303D7 (collection slot [9]).
//
// NO SetGlobalName (m_bInstanceMethods=1, PROVEN from Init r9d): INSTANCED per-entity methods table --
// methods injected into pickable ITEM ENTITY script tables (world item entities; <itemEntity>:Fn()
// colon-call, successor of KCD1's PickableItem entity binding; exact attach mechanism inferred).
// Handlers are DIRECT member-fn delegates; arg lists below are the binary's own SUserFunctionDesc
// param strings (() = registered with the shared empty param string).
//
// Lua fn table (38 fns; registrar sub_180EF81DC, called from ctor):
//   SetExtensionActivation  0x182AC24D0  (extension, bActivate)
//   SetExtensionParams      0x182AC260C  (extension, params)
//   GetExtensionParams      0x182ABB858  (extension, params)
//   GetStats                0x182ABD7A8  ()   returns m_pStatsTable when the calling entity resolves (gate 0x18143DC34), else no result
//   GetParams               0x182ABC894  ()   returns m_pParamsTable
//   Reset                   0x182AC179C  ()
//   Select                  0x182AC1C48  (select)
//   CanPickUp               0x182AB832C  (userId)
//   CanUse                  0x18143DAD4  (userId)
//   IsMounted               0x182ABE55C  ()
//   PlayAnimation           0x182AC0898  (animationName)
//   GetOwnerId              0x182ABC848  ()
//   GetEntityHoldingInHand  0x18143DB1C  ()
//   StartUse                0x182AC36FC  (userId)
//   StopUse                 0x182AC39D0  (userId)
//   Use                     0x182AC3FF0  (userId)
//   IsUsed                  0x182ABE980  ()
//   GetMountedDir           0x182ABC80C  ()
//   GetMountedAngleLimits   0x182ABC7A4  ()
//   SetMountedAngleLimits   0x180C820A0  (min_pitch, max_pitch, yaw_range)   NO-OP STUB
//   OnHit                   0x182ABFD54  (hit)
//   IsDestroyed             0x182ABDFAC  ()
//   IsOversized             0x182ABE62C  ()
//   IsFromShop              0x182ABE1B4  ()
//   BelongsToDeadBody       0x1811CDA9C  ()
//   GetLinkedOwner          0x182ABC484  ()
//   OnUsed                  0x182AC0054  (userId)
//   CanSteal                0x182AB8494  (userId)
//   OnSteal                 0x182ABFFD8  (userId)
//   GetSlot                 0x182ABD0EC  (usePresence)
//   GetHealth               0x180C820A0  ()   NO-OP STUB
//   GetMaxHealth            0x180C820A0  ()   NO-OP STUB
//   GetUIName               0x182ABDBB4  ()
//   GetId                   0x182ABBF38  ()
//   SetPhase                0x182AC2A50  (phase)
//   SetPhaseById            0x182AC2AC8  (id)
//   GetPhase                0x182ABC8A8  ()
//   GetPhaseById            0x182ABC8FC  ()
// NO-OP STUB = shared handler 0x180C820A0 (pH->EndFunction() only): registered in retail but does
// nothing and returns no values (GetHealth/GetMaxHealth/SetMountedAngleLimits).

namespace wh::entitymodule {

class C_ScriptBindPickableItem : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindPickableItem;
    void*                   m_pSystem;        // +0x60  = (*(S_GameContext+0x8))->vf[19]() (factory a2; same framework obj C_ScriptBindInventory +0x60 holds)
    Offsets::IScriptSystem* m_pScriptSystem;  // +0x68  = m_pSystem->vf(+0x278)() -- duplicates CScriptableBase::m_pSS (+0x50)
    void*                   m_pCtx8;          // +0x70  = *(S_GameContext+0x8) itself (factory a3; pointee UNRESOLVED). Handlers resolve the calling entity through it: vf(+0xD0)() -> vf(+0xA8)(entityId) (GetStats gate 0x18143DC34)
    Offsets::IScriptTable*  m_pStatsTable;    // +0x78  SmartScriptTable (ctor: pSS CreateTable+AddRef via helper 0x180AED4A8); the table GetStats returns
    Offsets::IScriptTable*  m_pParamsTable;   // +0x80  SmartScriptTable; the table GetParams returns
};
static_assert(sizeof(C_ScriptBindPickableItem) == 0x88, "C_ScriptBindPickableItem must be 0x88");

}  // namespace wh::entitymodule
