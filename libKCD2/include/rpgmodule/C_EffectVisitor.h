#pragma once

// -----------------------------------------------
// wh::rpgmodule::C_EffectVisitor -- mutable effect visitor interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184CE4C20, no own vtable (abstract).  38 slots (PROVEN: the only concrete vtable,
// C_GatherStateEffectVisitor 0x183A4B398, holds 38 function slots -- all the shared nullsub
// 0x1803B6E80 except [30] = its SoulState gatherer).  Slot indices recovered from every
// effect's Accept ([1]) thunk (`call [visitor+8*slot]`, COMDAT-folded per index; dump in
// analysis/effects_cluster/waveB/accept_thunks/).  NO virtual dtor -- slot 0 is a Visit
// (C_SoulPlaceholderEffect dispatches to vtbl[0]).
// C_DefaultEffectVisitor (TD 0x184CE40F0, vtable optimized out) is the all-no-op intermediate.
// Slots [21]/[35]/[37] belong to effect types never instantiated in retail (no TDs; names
// unrecoverable).  Return type of the Visit slots UNVERIFIED (thunks forward RAX); declared
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

class C_EffectVisitor {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EffectVisitor;
    virtual void VisitSoulPlaceholder(C_SoulPlaceholderEffect&) = 0;                          // [0]
    virtual void VisitProfilingLog(C_ProfilingLogEffect&) = 0;                                // [1]
    virtual void VisitLogPlayerReputation(C_LogPlayerReputationEffect&) = 0;                  // [2]
    virtual void VisitItemDestroyed(C_ItemDestroyedEffect&) = 0;                              // [3]
    virtual void VisitDmgR(C_DmgREffect&) = 0;                                                // [4]
    virtual void VisitSoulStatsLog(C_SoulStatsLogEffect&) = 0;                                // [5]
    virtual void VisitRestartStaminaRegenCooldown(C_RestartStaminaRegenCooldownEffect&) = 0;  // [6]
    virtual void VisitResetNearbyPublicFriendsReputation(C_ResetNearbyPublicFriendsReputationEffect&) = 0;  // [7]
    virtual void VisitResetPublicFriendsReputation(C_ResetPublicFriendsReputationEffect&) = 0;  // [8]
    virtual void VisitOneShotSoundReport(C_OneShotSoundReportEffect&) = 0;                    // [9]
    virtual void VisitSkillCheckResult(C_SkillCheckResultEffect&) = 0;                        // [10]
    virtual void VisitUnlockAchievement(C_UnlockAchievementEffect&) = 0;                      // [11]
    virtual void VisitFactionReputation(C_FactionReputationEffect&) = 0;                      // [12]
    virtual void VisitPerkUsed(C_PerkUsedEffect&) = 0;                                        // [13]
    virtual void VisitLearnPerk(C_LearnPerkEffect&) = 0;                                      // [14]
    virtual void VisitAddPerk(C_AddPerkEffect&) = 0;                                          // [15]
    virtual void VisitStealItems(C_StealItemsEffect&) = 0;                                    // [16]
    virtual void VisitAddItem(C_AddItemEffect&) = 0;                                          // [17]
    virtual void VisitRemoveAllBuffsByAITag(C_RemoveAllBuffsByAITagEffect&) = 0;              // [18]
    virtual void VisitRemoveAllBuffsByGuid(C_RemoveAllBuffsByGuidEffect&) = 0;                // [19]
    virtual void VisitAddBuff(C_AddBuffEffect&) = 0;                                          // [20]
    virtual void _VisitUnknown21(C_Effect&) = 0;                                              // [21] type never instantiated
    virtual void VisitProjectileStatus(C_ProjectileStatusEffect&) = 0;                        // [22]
    virtual void VisitDryHerb(C_DryHerbEffect&) = 0;                                          // [23]
    virtual void VisitPrepareFood(C_PrepareFoodEffect&) = 0;                                  // [24]
    virtual void VisitItemBlood(C_ItemBloodEffect&) = 0;                                      // [25]
    virtual void VisitRemoveItemOnEquipBuff(C_RemoveItemOnEquipBuffEffect&) = 0;              // [26]
    virtual void VisitAddItemOnEquipBuff(C_AddItemOnEquipBuffEffect&) = 0;                    // [27]
    virtual void VisitItemHealthValue(C_ItemHealthValueEffect&) = 0;                          // [28]
    virtual void VisitSoulBleedingHeal(C_SoulBleedingHealEffect&) = 0;                        // [29]
    virtual void VisitSoulState(C_SoulStateEffect&) = 0;                                      // [30] Health/Food/Alcoholism/ActStamina/Vigour too
    virtual void VisitStatXP(C_StatXPEffect&) = 0;                                            // [31]
    virtual void VisitSkillXP(C_SkillXPEffect&) = 0;                                          // [32]
    virtual void VisitSoulReputation(C_SoulReputationEffect&) = 0;                            // [33]
    virtual void VisitBlood(C_BloodEffect&) = 0;                                              // [34]
    virtual void _VisitUnknown35(C_Effect&) = 0;                                              // [35] type never instantiated
    virtual void VisitRuntimeStateFlag(C_RuntimeStateFlagEffect&) = 0;                        // [36]
    virtual void _VisitUnknown37(C_Effect&) = 0;                                              // [37] type never instantiated
};
static_assert(sizeof(C_EffectVisitor) == 0x08, "C_EffectVisitor is a vfptr-only interface");

}  // namespace wh::rpgmodule
