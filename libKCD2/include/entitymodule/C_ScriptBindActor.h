#pragma once
#include <cstdint>
#include "../crysystem/CScriptableBase.h"
#include "../crysystem/CScriptTable.h"

// -----------------------------------------------
// wh::entitymodule::C_ScriptBindActor -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88 (CONFIRMED: alloc 136).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindActor@entitymodule@wh@@ (TD 0x184C758C8, COL 0x184331638; CHD = [self,
// CScriptableBase])  vftable 0x183ECB620 (106 slots)  ctor sub_181289FD0  factory sub_180E30204.
//
// NO SetGlobalName call: per-entity bind; methods attach to actor/NPC entity script tables at
// bind time (the Lua `actor` table on actor entities -- access path inferred, attach site not
// traced). Handlers are pointer-to-virtual-member thunks resolved against THIS vtable (sN = slot).
// ICF-folded pairs share one body: IsPlayer/IsLocalClient, ResetScores/RenderScore.
//
// Lua fn table `actor` (110 fns; registered in ctor):
//   Revive                       0x182AC1A54  s5   (fromGameModeInit)   *** CHEAT: resurrect ***
//   StandUp                      0x182AC3268  s6   ()
//   RagDollize                   0x182AC0C88  s7   ()
//   PostPhysicalize              0x182AC0934  s8   ()
//   GetLinkedEntity              0x182ABC3EC  s10  ()
//   RequestDialog                0x1804D1834  s11  ()
//   CanTalk                      0x18082A578  s12  ()
//   CanChat                      0x18082B6F8  s13  ()
//   HasChatRequest               0x18143485C  s14  ()
//   HasAcceptedChat              0x1813C49F8  s15  ()
//   AcceptChatRequest            0x182AB764C  s16  ()
//   DoChat                       0x1804D07E8  s17  ()
//   OpenChat                     0x182AC00A4  s18  ()
//   IsWaitingForDialogueReply    0x182ABE9C8  s19  ()
//   CanFollow                    0x18082D028  s20  ()
//   StartFollow                  0x182AC3624  s21  ()
//   IsFollowing                  0x182ABE100  s22  ()
//   SimulateOnAction             0x182AC3010  s23  (action, mode, value)
//   SetPhysicalizationProfile    0x182AC2B38  s24  (profile)
//   GetPhysicalizationProfile    0x182ABC978  s25  ()
//   GetFrozenAmount              0x182ABB9D0  s26  ()
//   AddFrost                     0x182AB7694  s27  (frost)
//   DumpActorInfo                0x182AB9DB4  s28  ()
//   SetMovementRestriction       0x182AC29FC  s29  (bAllowSprint, bAllowRun)
//   SetLookIK                    0x182AC28F0  s30  (enable)
//   GetHeadDir                   0x182ABBA0C  s31  ()
//   GetHeadPos                   0x182ABBAD8  s32  ()
//   GetChannel                   0x182ABA92C  s33  ()
//   IsPlayer                     0x182ABE448  s34  ()
//   IsLocalClient                0x182ABE448  s35  ()   (ICF-folded with IsPlayer)
//   IsGhostPit                   0x182ABE220  s36  ()
//   IsFlying                     0x182ABE08C  s37  ()
//   SetAngles                    0x182AC22A0  s38  (angles)
//   GetAngles                    0x182ABA820  s39  ()
//   PlayerSetViewAngles          0x1819ADEA8  s40  (angles)
//   SetViewLimits                0x182AC2DA4  s41  (dir, rangeH, rangeV)
//   CameraShake                  0x182AB7FC0  s42  (amount, duration, frequency, pos)
//   SetViewShake                 0x182AC2E1C  s43  (shakeAngle, shakeShift, duration, frequency, randomness)
//   VectorToLocal                0x182AC402C  s44  ()
//   EnableAspect                 0x182AB9F94  s45  (aspects, enable)
//   SetExtensionActivation       0x182AC243C  s46  (extension, bActivate)
//   SetExtensionParams           0x182AC2564  s47  (?)
//   GetExtensionParams           0x182ABB7B0  s48  (?)
//   SetHealth                    0x182AC2824  s49  (health)             *** CHEAT: set HP ***
//   SetMaxHealth                 0x182AC2928  s50  (health)             *** CHEAT ***
//   GetHealth                    0x18178586C  s51  ()
//   IsUnconscious                0x18082A540  s52  ()
//   IsDead                       0x18082A85C  s53  ()
//   GetMaxHealth                 0x182ABC6F8  s54  ()
//   GetArmor                     0x182ABA89C  s55  ()
//   GetMaxArmor                  0x182ABC6B0  s56  ()
//   SetMovementControlledByAnimation 0x182AC2968 s57 (enable)
//   CreateCodeEvent              0x182AB8834  s58  (params)
//   CanInteractWith              0x18082B7EC  s59  (entityId)
//   GetCurrentAnimationState     0x182ABB5C8  s60  ()
//   TrackViewControlled          0x182AC3D7C  s61  (enable)
//   Fall                         0x182ABA1C8  s62  (hitPosX, hitPosY, hitPosZ)
//   GetClosestAttachment         0x182ABADB8  s63  (characterSlot, testPos, maxDistance, suffix)
//   AttachVulnerabilityEffect    0x182AB7C88  s64  (6 args)
//   ResetVulnerabilityEffects    0x182AC1990  s65  ()
//   GetCloseColliderParts        0x182ABAA18  s66  (characterSlot, hitPos, radius)
//   ResetScores                  0x182AC1188  s67  ()
//   RenderScore                  0x182AC1188  s68  ()   (ICF-folded with ResetScores)
//   SetSearchBeam                0x180C820A0  s69  (dir)
//   SetSpectatorMode             0x182AC2D04  s70  (mode, target)
//   GetSpectatorMode             0x182ABD73C  s71  ()
//   SetDialogAnimationState      0x182AC2404  s72  (state)
//   CanStealthKill               0x18082A614  s75  (victimEntityId)
//   CanStealthKnockout           0x18082A67C  s76  (victimEntityId)
//   RequestStealthKill           0x182AC1794  s77  (victimEntityId)
//   RequestKnockOut              0x182AC1608  s78  (victimEntityId)
//   CanHorsePullDown             0x18082A6FC  s79  (victimEntityId)
//   RequestHorsePullDown         0x182AC1518  s80  (victimEntityId)
//   CanHuntAttack                0x182AB8274  s81  (victimEntityId)
//   RequestHuntAttack            0x182AC1590  s82  (victimEntityId)
//   CanDoMercyKill               0x181604378  s83  (victimEntityId)
//   RequestMercyKill             0x182AC1610  s84  (victimEntityId)
//   IsCarryingCorpse             0x180B26BB8  s85  ()
//   CanGrabCorpse                0x18082A764  s86  (victimEntityId)
//   RequestGrabCorpse            0x182AC14D0  s87  (victimEntityId)
//   CanPutCorpse                 0x182AB83B0  s88  ()
//   RequestPutCorpse             0x182AC16C8  s89  ()
//   CanLoot                      0x18154F9E0  s90  (victimEntityId)
//   HasItemsInInventory          0x181F277C0  s91  ()
//   CanBeButchered               0x182AB80A8  s92  ()
//   IsPlayerInButcheringDistance 0x182ABE678  s93  ()
//   EquipInventoryItem           0x182ABA18C  s94  (itemId)
//   UnequipInventoryItem         0x182AC3DEC  s95  (itemId)
//   EquipClothingPreset          0x1823DCEC8  s96  (presetGuid)
//   EquipWeaponPreset            0x1823DCDDC  s97  (presetGuid)
//   StartInteractiveActionByName 0x180702224  s98  (ActionName, ObjectId, UpdateVisibility, AnimSpeed)
//   RequestItemExchange          0x18194BD90  s99  ()
//   MakeLookAsActor              0x182ABEED8  s100 (sourceEntityGuid, inHand)
//   MakeLookAsSoul               0x182ABEF6C  s101 (sourceSoulGuid)
//   OpenItemSelectionFilter      0x182AC0118  s102 (?)
//   OpenItemMultiselectionFilter 0x182AC0104  s103 (?)
//   OpenItemSelectionSharpening  0x182AC028C  s104 (entityId)
//   OpenItemTransferStore        0x181F27BC0  s105 (otherInvenotryId, filter, uiHeaderName)
//   Butcher                      0x181F27590  direct (victimEntityId)
//   SetForcedLookDir             0x182AC276C  direct (dir)
//   ClearForcedLookDir           0x182AB87A4  direct ()
//   SetForcedLookObjectId        0x182AC27E0  direct (objectId)
//   ClearForcedLookObjectId      0x182AB87D8  direct ()
//   AddDirt                      0x18178E9C0  direct (dirtDelta)
//   CleanDirt                    0x182AB86E0  direct ()
//   WashDirtAndBlood             0x182AC4114  direct ()
//   WashItems                    0x182AC4180  direct (maxCleaningEffect)
//   IsMoreDirty                  0x180AB2AF4  direct (dirtLevel)
//   IsBodyMoreDirty              0x180AB2B48  direct (dirtLevel)
//   AddBlood                     0x180AD7D98  direct (bloodDelta)
// Registered constants (global table qword_18492D828): STANCE_ZEROG=9, E_ACTORSTANCE_COMBAT=0,
// E_ACTORSTANCE_CROUCH=1, E_ACTORSTANCE_STONETHROWING=2, E_ACTORSTANCE_NORMAL=3,
// E_ACTORSTANCE_INJURED=7, E_ACTORSTANCE_CARRYCORPSE=9. Into m_pSS: SAT_Undefined=0,
// SAT_KillDisabled=1, SAT_KnockoutDisabled=2, SAT_KillEnabled=3, SAT_KnockoutEnabled=4,
// SAT_KillDisabledNoDagger=5; HPS_*/HAS_*/MKS_* = 0/1/2 (Undefined/Disabled/Enabled).

namespace wh::entitymodule {

class C_ScriptBindActor : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindActor;
    ::CScriptTable* m_pParamTable;   // +0x60  ref-counted SmartScriptTable (released via sub_1804FB980
                                     //        in dtor sub_182AB71D0; purpose name INFERRED)
    Offsets::ISystem* m_pSystemArg;  // +0x68  ctor a2 = ISystem*; a2->vf[0x278] returns gEnv->pScriptSystem, fed to CScriptableBase::Init
    void*           m_pSubsystem;    // +0x70  = ISystem::vf[0x2C8](slot89 -> gEnv->pGame/IGame*)->IGame::vf16() (sub_180645968) = *(whGlobal sub_1809155C8()+8); engine-root obj, no static RTTI
    void*           m_pArg3;         // +0x78  ctor a3 = script-bindings container (factory sub_180E30204 arg a1; stores binds at container+0x10/+0x18/+0x20); non-owning back-ptr
    int32_t         m_field80;       // +0x80  (=0; role unresolved)
    uint32_t        _pad84;          // +0x84
};
static_assert(sizeof(C_ScriptBindActor) == 0x88, "C_ScriptBindActor must be 0x88");
static_assert(offsetof(C_ScriptBindActor, m_field80) == 0x80, "last field at 0x80");

}  // namespace wh::entitymodule
