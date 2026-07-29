#pragma once
#include <cstdint>
#include <cstddef>
#include "C_PerkList.h"                          // I_EventListener lives there
#include "I_LocationListener.h"
#include "../entitymodule/I_InventoryListener.h"
#include "C_DefaultCauseVisitor.h"
#include "C_DefaultConstEffectVisitor.h"

// -----------------------------------------------
// wh::rpgmodule::C_EventCounter -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x7F8.
// -----------------------------------------------
// RTTI .?AVC_EventCounter@rpgmodule@wh@@ (TD 0x184D8DBD8; COLs at +0x00/+0x08/+0x10/+0x18/
// +0x20 match the five bases below).  Vtables 0x183B091B8 head.  ctor sub_1810E3574.
// The statistics module's game-event intake: listens on every channel (events, locations,
// inventory, causes, effects) and feeds the statistic counters.  Sole instance is embedded
// INLINE in C_Statistics @+0x80 -- the sizeof is inferred from that embedding (0x80 + 0x7F8 =
// the +0x878 tail word, and the ctor's last store is the qword at +0x7F0); no independent
// allocation site exists [size INFERRED].
// Body fields are almost entirely un-RE'd (nothing in the alchemy work reads them) -- the two
// unmapped regions below are bounded and offset-checked, not semantic claims.

namespace wh::rpgmodule {

class C_EventCounter
    : public I_EventListener                       // +0x00
    , public I_LocationListener                    // +0x08
    , public wh::entitymodule::I_InventoryListener // +0x10
    , public C_DefaultCauseVisitor                 // +0x18
    , public C_DefaultConstEffectVisitor           // +0x20
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EventCounter;

    // Concrete in retail -- the base pure slots resolve to no-op/forwarder overrides on the
    // five subobject vtables (0x183B091B8..).  Declared-not-defined here (this replica is
    // never instantiated); listed only so the inline embedding in C_Statistics compiles.
    void _vf0() override;   // I_LocationListener [0] (same signature also satisfies C_CauseVisitor::_vf0)
    void _vf1() override;
    void _vf2() override;
    void _vf3() override;
    void _vf4() override;
    void _vf5() override;
    void _vf6() override;
    void OnRegistered(I_RPGLocationManager* mgr) override;
    void OnUnregistered(I_RPGLocationManager* mgr) override;
    void OnLocationOccupied(I_RPGLocationManager* mgr, I_Location* loc) override;
    void VisitSoulPlaceholder(const C_SoulPlaceholderEffect&) override;          // C_ConstEffectVisitor
    void VisitProfilingLog(const C_ProfilingLogEffect&) override;
    void VisitLogPlayerReputation(const C_LogPlayerReputationEffect&) override;
    void VisitItemDestroyed(const C_ItemDestroyedEffect&) override;
    void VisitDmgR(const C_DmgREffect&) override;
    void VisitSoulStatsLog(const C_SoulStatsLogEffect&) override;
    void VisitRestartStaminaRegenCooldown(const C_RestartStaminaRegenCooldownEffect&) override;
    void VisitDeniedAction(const C_DeniedActionEffect&) override;
    void VisitResetNearbyPublicFriendsReputation(const C_ResetNearbyPublicFriendsReputationEffect&) override;
    void VisitResetPublicFriendsReputation(const C_ResetPublicFriendsReputationEffect&) override;
    void VisitOneShotSoundReport(const C_OneShotSoundReportEffect&) override;
    void VisitSkillCheckResult(const C_SkillCheckResultEffect&) override;
    void VisitUnlockAchievement(const C_UnlockAchievementEffect&) override;
    void VisitFactionReputation(const C_FactionReputationEffect&) override;
    void VisitPerkUsed(const C_PerkUsedEffect&) override;
    void VisitLearnPerk(const C_LearnPerkEffect&) override;
    void VisitAddPerk(const C_AddPerkEffect&) override;
    void VisitStealItems(const C_StealItemsEffect&) override;
    void VisitAddItem(const C_AddItemEffect&) override;
    void VisitRemoveAllBuffsByAITag(const C_RemoveAllBuffsByAITagEffect&) override;
    void VisitRemoveAllBuffsByGuid(const C_RemoveAllBuffsByGuidEffect&) override;
    void VisitAddBuff(const C_AddBuffEffect&) override;
    void _VisitUnknown22(const C_Effect&) override;
    void VisitProjectileStatus(const C_ProjectileStatusEffect&) override;
    void VisitDryHerb(const C_DryHerbEffect&) override;
    void VisitPrepareFood(const C_PrepareFoodEffect&) override;
    void VisitItemBlood(const C_ItemBloodEffect&) override;
    void VisitRemoveItemOnEquipBuff(const C_RemoveItemOnEquipBuffEffect&) override;
    void VisitAddItemOnEquipBuff(const C_AddItemOnEquipBuffEffect&) override;
    void VisitItemHealthValue(const C_ItemHealthValueEffect&) override;
    void VisitSoulBleedingHeal(const C_SoulBleedingHealEffect&) override;
    void VisitSoulState(const C_SoulStateEffect&) override;
    void VisitStatXP(const C_StatXPEffect&) override;
    void VisitSkillXP(const C_SkillXPEffect&) override;
    void VisitSoulReputation(const C_SoulReputationEffect&) override;
    void VisitBlood(const C_BloodEffect&) override;
    void _VisitUnknown36(const C_Effect&) override;
    void VisitRuntimeStateFlag(const C_RuntimeStateFlagEffect&) override;
    void _VisitUnknown38(const C_Effect&) override;

    uint8_t  m_flag28;          // +0x28  ctor 0
    uint8_t  _pad29[7];         // +0x29
    void*    m_default30;       // +0x30  ctor = qword_1853323F0 (shared default object)
    int32_t  m_int38;           // +0x38  ctor 0
    uint32_t _pad3C;            // +0x3C
    void*    m_ptr40;           // +0x40  ctor 0
    void*    m_ptr48;           // +0x48  ctor 0
    int32_t  m_int50;           // +0x50  ctor -1
    uint8_t  m_flag54;          // +0x54  ctor 0
    uint8_t  _unk55[0x248 - 0x55];   // +0x55  ctor-zeroed listener state, unmapped
    uint8_t  _unk248[0x7F8 - 0x248]; // +0x248 ctor-zeroed; 
};
static_assert(offsetof(C_EventCounter, m_flag28) == 0x28, "own fields start at 0x28");
static_assert(sizeof(C_EventCounter) == 0x7F8, "C_EventCounter must be 0x7F8");

}  // namespace wh::rpgmodule
