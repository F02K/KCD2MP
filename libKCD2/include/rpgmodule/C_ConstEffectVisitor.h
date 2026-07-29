#pragma once

// -----------------------------------------------
// wh::rpgmodule::C_ConstEffectVisitor -- const effect visitor interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184CE8DC0, no own vtable (abstract).  Same slot order as C_EffectVisitor with ONE
// extra slot inserted at [7]: every effect's AcceptConst thunk index equals its Accept index
// for slots 0-6 and Accept+1 from slot 7 on.  The insert is VisitDeniedAction [INFERRED:
// C_DeniedActionEffect has a S_ConstVisitorSpecHelper instantiation but NO mutable helper and
// no TD/vtable -- const-only visitable, never instantiated in retail].  39 slots total
// [count INFERRED: mutable count 38 (proven) + the insert; no concrete const vtable exists to
// count directly].  C_DefaultConstEffectVisitor (TD 0x184CE8F50) is the all-no-op intermediate.
// NO virtual dtor -- slot 0 is a Visit.  Return type of the Visit slots UNVERIFIED; declared
// void -- layout-neutral.

namespace wh::rpgmodule {

class C_Effect;
class C_SoulPlaceholderEffect;
class C_ProfilingLogEffect;
class C_LogPlayerReputationEffect;
class C_ItemDestroyedEffect;
class C_DmgREffect;
class C_SoulStatsLogEffect;
class C_RestartStaminaRegenCooldownEffect;
class C_DeniedActionEffect;   // no TD/vtable in retail; known only from the const spec helper
class C_ResetNearbyPublicFriendsReputationEffect;
class C_ResetPublicFriendsReputationEffect;
class C_OneShotSoundReportEffect;
class C_SkillCheckResultEffect;
class C_UnlockAchievementEffect;
class C_FactionReputationEffect;
class C_PerkUsedEffect;
class C_LearnPerkEffect;
class C_AddPerkEffect;
class C_StealItemsEffect;
class C_AddItemEffect;
class C_RemoveAllBuffsByAITagEffect;
class C_RemoveAllBuffsByGuidEffect;
class C_AddBuffEffect;
class C_ProjectileStatusEffect;
class C_DryHerbEffect;
class C_PrepareFoodEffect;
class C_ItemBloodEffect;
class C_RemoveItemOnEquipBuffEffect;
class C_AddItemOnEquipBuffEffect;
class C_ItemHealthValueEffect;
class C_SoulBleedingHealEffect;
class C_SoulStateEffect;
class C_StatXPEffect;
class C_SkillXPEffect;
class C_SoulReputationEffect;
class C_BloodEffect;
class C_RuntimeStateFlagEffect;

class C_ConstEffectVisitor {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ConstEffectVisitor;
    virtual void VisitSoulPlaceholder(const C_SoulPlaceholderEffect&) = 0;                          // [0]
    virtual void VisitProfilingLog(const C_ProfilingLogEffect&) = 0;                                // [1]
    virtual void VisitLogPlayerReputation(const C_LogPlayerReputationEffect&) = 0;                  // [2]
    virtual void VisitItemDestroyed(const C_ItemDestroyedEffect&) = 0;                              // [3]
    virtual void VisitDmgR(const C_DmgREffect&) = 0;                                                // [4]
    virtual void VisitSoulStatsLog(const C_SoulStatsLogEffect&) = 0;                                // [5]
    virtual void VisitRestartStaminaRegenCooldown(const C_RestartStaminaRegenCooldownEffect&) = 0;  // [6]
    virtual void VisitDeniedAction(const C_DeniedActionEffect&) = 0;                                // [7] const-only [INFERRED]
    virtual void VisitResetNearbyPublicFriendsReputation(const C_ResetNearbyPublicFriendsReputationEffect&) = 0;  // [8]
    virtual void VisitResetPublicFriendsReputation(const C_ResetPublicFriendsReputationEffect&) = 0;  // [9]
    virtual void VisitOneShotSoundReport(const C_OneShotSoundReportEffect&) = 0;                    // [10]
    virtual void VisitSkillCheckResult(const C_SkillCheckResultEffect&) = 0;                        // [11]
    virtual void VisitUnlockAchievement(const C_UnlockAchievementEffect&) = 0;                      // [12]
    virtual void VisitFactionReputation(const C_FactionReputationEffect&) = 0;                      // [13]
    virtual void VisitPerkUsed(const C_PerkUsedEffect&) = 0;                                        // [14]
    virtual void VisitLearnPerk(const C_LearnPerkEffect&) = 0;                                      // [15]
    virtual void VisitAddPerk(const C_AddPerkEffect&) = 0;                                          // [16]
    virtual void VisitStealItems(const C_StealItemsEffect&) = 0;                                    // [17]
    virtual void VisitAddItem(const C_AddItemEffect&) = 0;                                          // [18]
    virtual void VisitRemoveAllBuffsByAITag(const C_RemoveAllBuffsByAITagEffect&) = 0;              // [19]
    virtual void VisitRemoveAllBuffsByGuid(const C_RemoveAllBuffsByGuidEffect&) = 0;                // [20]
    virtual void VisitAddBuff(const C_AddBuffEffect&) = 0;                                          // [21]
    virtual void _VisitUnknown22(const C_Effect&) = 0;                                              // [22] type never instantiated
    virtual void VisitProjectileStatus(const C_ProjectileStatusEffect&) = 0;                        // [23]
    virtual void VisitDryHerb(const C_DryHerbEffect&) = 0;                                          // [24]
    virtual void VisitPrepareFood(const C_PrepareFoodEffect&) = 0;                                  // [25]
    virtual void VisitItemBlood(const C_ItemBloodEffect&) = 0;                                      // [26]
    virtual void VisitRemoveItemOnEquipBuff(const C_RemoveItemOnEquipBuffEffect&) = 0;              // [27]
    virtual void VisitAddItemOnEquipBuff(const C_AddItemOnEquipBuffEffect&) = 0;                    // [28]
    virtual void VisitItemHealthValue(const C_ItemHealthValueEffect&) = 0;                          // [29]
    virtual void VisitSoulBleedingHeal(const C_SoulBleedingHealEffect&) = 0;                        // [30]
    virtual void VisitSoulState(const C_SoulStateEffect&) = 0;                                      // [31] Health/Food/Alcoholism/ActStamina/Vigour too
    virtual void VisitStatXP(const C_StatXPEffect&) = 0;                                            // [32]
    virtual void VisitSkillXP(const C_SkillXPEffect&) = 0;                                          // [33]
    virtual void VisitSoulReputation(const C_SoulReputationEffect&) = 0;                            // [34]
    virtual void VisitBlood(const C_BloodEffect&) = 0;                                              // [35]
    virtual void _VisitUnknown36(const C_Effect&) = 0;                                              // [36] type never instantiated
    virtual void VisitRuntimeStateFlag(const C_RuntimeStateFlagEffect&) = 0;                        // [37]
    virtual void _VisitUnknown38(const C_Effect&) = 0;                                              // [38] mirrors mutable [37]
};
static_assert(sizeof(C_ConstEffectVisitor) == 0x08, "C_ConstEffectVisitor is a vfptr-only interface");

}  // namespace wh::rpgmodule
