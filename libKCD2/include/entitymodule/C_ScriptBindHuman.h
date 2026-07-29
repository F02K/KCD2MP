#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"
#include "../crysystem/CScriptTable.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindHuman -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78 (CONFIRMED: alloc 120).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindHuman@entitymodule@wh@@ (TD 0x184C75890, COL 0x184331700; CHD = [self,
// CScriptableBase])  vftable 0x183ECB3D0  ctor sub_18128B9DC  dtor sub_182AB72E8  factory sub_180E30204.
//
// NO SetGlobalName: per-entity bind; methods attach to human entity script tables (e.g.
// entity.human -- access path inferred from the entity-bind pattern). Handlers are
// pointer-to-virtual-member thunks resolved against THIS vtable (offset = vtable byte offset).
//
// Lua fn table (38 fns; registered in ctor; full vtable-resolution verified):
//   RequestDialog               0x182AC12D4  +0x28   ()
//   CanTalk                     0x182AB84E4  +0x30   ()
//   CanInteractWith             0x182AB82DC  +0x38   (entityId)
//   IsInDialog                  0x182ABE29C  +0x40   ()
//   InterruptDialogs            0x182ABDE7C  +0x48   ()
//   GetDialogRequestSourceName  0x182ABB630  +0x50   ()
//   PickUpItem                  0x182AC05F0  +0x58   (itemId, toHand)
//   PlaceItem                   0x182AC06A4  +0x60   (itemId, entity, fromHand)
//   ToggleWeapon                0x182AC3C5C  +0x68   ()
//   DrawWeapon                  0x182AB9CEC  +0x70   ()
//   ToggleWeaponSet             0x182AC3CF0  +0x78   (set)
//   HolsterWeapon               0x182ABDE04  +0x80   ()
//   AttachEntityToHand          0x182AB7C18  +0x88   (id, hand)
//   DetachFromHand              0x182AB8D0C  +0x90   (hand)
//   IsWeaponDrawn               0x182ABEA64  +0x98   ()
//   DrawFromInventory           0x182AB9C90  +0xA0   (itemId, hand, bAnimate)
//   HolsterToInventory          0x182ABDDB8  +0xA8   (hand, bAnimate)
//   GetTorchLightArchetype      0x1804CECB0  +0xB0   ()
//   GrabOnLadder                0x182ABDC94  +0xB8   (ladderId)
//   IsOnLadder                  0x182ABE5E4  +0xC0   ()
//   CanUseLadder                0x18197D608  +0xC8   (ladderId, ladderHeight, useOnlyFromFront)
//   Mount                       0x1819A61CC  +0xD0   (horseId)
//   Dismount                    0x182AB8D64  +0xD8   ()
//   DoBonding                   0x182AB9C04  +0xE0   (horseId)
//   GetHorseMountPoint          0x182ABBD38  +0xE8   (horseId)
//   ForceMount                  0x182ABA5F8  +0xF0   (horseId)
//   ForceDismount               0x182ABA5C4  +0xF8   ()
//   StartReading                0x182AC36C4  +0x100  (bookId)
//   GetItemInHand               0x182ABC1F8  +0x108  (handId)
//   PlayAnim                    0x182AC0834  +0x110  (fragment, tag)
//   StopAnim                    0x182AC3944  +0x118  ()
//   SetAnimMotionParam          0x182AC22E0  +0x120  (paramId, value)
//   IsMounted                   0x182ABE524  +0x128  ()
//   GetHorse                    0x182ABBCD0  +0x130  ()
//   RequestPickpocketing        0x182AC1664  +0x138  (victimEntityId)
//   CanBeRobbed                 0x1804CECE8  +0x140  (victimEntityId)
//   IsPickpocketing             0x1804CED34  +0x148  ()
//   PrepareFood                 0x182AC0964  +0x150  (itemId, type)
// Registered constants: EP_DISCARD=0, EP_FOLD=1, WS_PRIMARY=0, WS_SECONDARY=1, HS_LEFT=1,
// HS_RIGHT=0, PFT_COOK=0, PFT_SMOKE=1, PFT_DRY=2.

namespace wh::entitymodule {

class C_ScriptBindHuman : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindHuman;
    ::CScriptTable* m_pParamTable;   // +0x60  obj created via sub_180AED4A8, released via sub_1804FB980 (INFERRED SmartScriptTable)
    void*           m_pContext;      // +0x68  = ctor a2 = ctx = (S_GameContext+8)->vf[19](); framework/service obj (ctx->vf[+0x278]=IScriptSystem=base m_pSS); class UNCONFIRMED
    void*           m_pManager;      // +0x70  = ctx->vf[+0x2C8]()->vf[+0x80]() (ctor sub_18128B9DC); manager obj, pointee UNRESOLVED
};
static_assert(sizeof(C_ScriptBindHuman) == 0x78, "C_ScriptBindHuman must be 0x78");

}  // namespace wh::entitymodule
