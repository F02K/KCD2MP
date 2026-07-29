#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"
#include "../crysystem/CScriptTable.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindPlayer -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78 (CONFIRMED: alloc 120).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindPlayer@entitymodule@wh@@ (TD 0x184C759C0, COL 0x1843316B0; CHD = [self,
// CScriptableBase])  vftable 0x183ECB1B8 (36 slots)  ctor sub_18128C9DC  factory sub_180E30204.
//
// NO SetGlobalName call: per-entity bind whose methods table attaches to the local-player entity
// script table -- surfaces as the Lua global `player` (access path inferred from KCD1 prior +
// confirmed console usage `player:GetPos()`; the attach site was not traced).
//
// Handlers are pointer-to-VIRTUAL-member: the registered thunk dispatches on this class's own
// vtable, so the SAME thunk resolves differently per bind class. Addresses below are the RESOLVED
// concrete implementations (sN = vtable slot N).
//
// Lua fn table `player` (33 fns; registered in ctor):
//   OnEnterInteractive        0x1819B49C4  s5   ()
//   OnEndInteractive          0x1819B4A44  s6   ()
//   OnSleeping                0x182ABFED0  s7   (sleepQuality)
//   EnableFastTravel          0x182ABA048  s8   (enable)
//   GetHorseId                0x1817592FC  s9   ()
//   SetPlayerHorse            0x181797264  s10  (entityId)
//   GetPlayerHorse            0x182ABCB6C  s11  ()
//   ClearPlayerHorse          0x182AB8804  s12  ()
//   SetWhistling              0x182AC2F18  s13  (activate)
//   RequestDogObjective       0x182AC146C  s15  (entityId)
//   CancelDogObjective        0x182AB8640  s16  ()
//   HasRunningDogObjective    0x182ABDD70  s17  ()
//   FeedDog                   0x182ABA348  s18  ()
//   AddLuaActions             0x180A2CFC0  s19  (actions)
//   AddSoAction               0x182AB7860  s20  (wuid, strPress, strHold, interaction, [enable], [reason])
//   PushBack                  0x182AC0A90  s23  (distance)
//   OnBedStop                 0x182ABF7F8  s24  ()
//   OnBedInterrupt            0x182ABF6F0  s25  ()
//   CanSleepAndReportProblem  0x182AB845C  s26  ()
//   CanSleep                  0x182AB8414  s27  ()
//   CanSleepFast              0x181F27630  s28  ()
//   CanChangeStanceObject     0x182AB81C8  s29  (stanceObjectId)
//   OnBedPrepareForDialog     0x182ABF770  s30  ()
//   IsSitting                 0x1817DFB68  s31  ()
//   IsLaying                  0x1817DFBA4  s32  ()
//   IsLockedToOpponent        0x182ABE494  s33  (entityId)
//   IsCombatChatTarget        0x18082A918  s34  (entityId)
//   HorseInspect              0x182ABDE40  s35  (horseId)
//   CanMountHorse             0x180AD4760  direct  (horseId)
//   CanBondHorse              0x182AB80F8  direct  (horseId)
//   SetFlyMode                0x182AC26B4  direct  (flyMode)     *** CHEAT: noclip/fly; 2 = no collisions ***
//   GetFlyMode                0x182ABB900  direct  ()
//   SetAlcoTeleportTarget     0x182AC2250  direct  (targetWuid)
// Registered constants: FlyMode_Off=0, FlyMode_On=1, FlyMode_OnNoCollisions=2,
// CanMount_Disable=0, CanMount_Enable=1, CanMount_DisableEncumbered=2.

namespace wh::entitymodule {

class C_ScriptBindPlayer : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindPlayer;
    ::CScriptTable* m_pParamTable;   // +0x60  ref-counted SmartScriptTable (CreateTable via sub_180AED4A8,
                                     //        released via sub_1804FB980 in dtor; purpose name INFERRED)
    void*           m_pSystemArg;    // +0x68  = ctor a2 = ctx = (S_GameContext+8)->vf[19](); framework/service obj (ctx->vf[+0x278]=IScriptSystem=base m_pSS); class UNCONFIRMED
    void*           m_pSubsystem;    // +0x70  = ctx->vf[+0x2C8]()->vf[+0x80]() (ctor 0x18128CA29/0x18128CA3E); same expr as Human m_pManager; pointee UNRESOLVED
};
static_assert(sizeof(C_ScriptBindPlayer) == 0x78, "C_ScriptBindPlayer must be 0x78");
static_assert(offsetof(C_ScriptBindPlayer, m_pParamTable) == 0x60, "param table at 0x60");

}  // namespace wh::entitymodule
