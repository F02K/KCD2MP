#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::I_RPGMinigames -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (vptr only).
// -----------------------------------------------
// RTTI .?AVI_RPGMinigames@rpgmodule@wh@@ (TD 0x184D5FC90).  Sole implementor C_RPGMinigames
// (base @+0x00, vtable 0x183C16A90, 83 slots to +0x290).  The per-player minigame reward/
// progress service: alchemy brew grading rewards, recipe knowledge, brewed-product tracking,
// plus the other minigames' slots (dice/archery/... -- the [15..82] tail, unmodeled).
//
// Alchemy cluster certified from the grade path (GrantBrewReward_182E163F4 §3.1), the item-use
// recipe-learn path (sub_182AB0008 <- dispatcher sub_181F23B30 / prompt flow sub_1811A0548) and
// the UI consumers (C_UIFlashListItem ctor sub_1808F67C4, unbrewed-product picker sub_181FD25A0)
// -- full decode: analysis/alchemy_re/balancing_harness_research.md "Brewed-once gate research".
//
// "Learned" state = sorted {recipeId, stepBits} vector (C_RPGMinigames+0x10): bit i = recipe
// step i known.  MarkRecipeLearned (the ONLY writer) sets ALL bits (0x1FFFFFFF); set both by the
// first successful brew AND by reading a recipe document => knowledge, NOT "brewed before".
// EVERYTHING here is knowledge/document state -- the "ever brewed" record lives elsewhere, in
// the UniquePotionsBrewed DistinctGuid statistic (C_Statistics.h).

namespace wh::rpgmodule {

class C_Soul;

class I_RPGMinigames {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RPGMinigames;

    virtual ~I_RPGMinigames() = default;                                      // [0]
    // Uncertified fillers -- DO NOT call through them.
    virtual void _vf1() = 0;                                                  // [1]
    virtual void _vf2() = 0;                                                  // [2]
    virtual void _vf3() = 0;                                                  // [3]
    virtual void _vf4() = 0;                                                  // [4]
    virtual void _vf5() = 0;                                                  // [5]
    virtual void _vf6() = 0;                                                  // [6]
    virtual void _vf7() = 0;                                                  // [7]
    // Sets ALL step bits (0x1FFFFFFF) for the recipe -- knowledge grant.  Callers: grade-success
    // (discovery-by-brewing) and the recipe-document item-use path.  Player souls only (non-
    // player: no-op / queries below return true).
    virtual void  MarkRecipeLearned(C_Soul* soul, uint32_t recipeId) = 0;     // [8]  +0x40  0x181326AAC
    // All step bits [0..stepCount] set?  TRUE for anything readable in the book (documents mark
    // it too) -- NOT a "brewed before" test.
    virtual bool  IsRecipeLearned(C_Soul* soul, uint32_t recipeId) = 0;       // [9]  +0x48  0x182CF84D0
    virtual bool  IsRecipeStepLearned(C_Soul* soul, uint32_t recipeId,
                                      uint32_t stepBit) = 0;                  // [10] +0x50  0x181326BEC
    virtual bool  AreRecipeStepsLearned(C_Soul* soul, uint32_t recipeId,
                                        uint32_t firstBit, uint32_t lastBit) = 0;  // [11] +0x58  0x182CF8534
    // The skill-lerped boil/heat tolerance band (certified from the impl):
    //   clamp01( soul->GetSkillFraction(6 /*alchemy*/) * (AlchemyToleranceMaxLevel -
    //            AlchemyToleranceBase) + AlchemyToleranceBase )
    // (S_Constants float idx 0x1A5/0x1A6).  apq (E_PerkStat 74) does NOT enter -- it only
    // feeds the quality coefficient X.  Consumed by the boil comparator 0x182E2B050 via the
    // zero-arg wrapper 0x182E2C594 (= the shipped C_Alchemy::GetBrewTolerance forwarder).
    virtual float GetBrewTolerance(C_Soul* soul) = 0;                         // [12] +0x60  0x182CF3FA0
    // Grade-path grant: spawns outCount x outProductId (failed brews spawn the generic "unknown
    // potion"), adds statistics + the AlchemyRecipeFinish cause; returns the pass verdict.
    // rewardDesc = the 0x2C-byte descriptor built in GrantBrewReward_182E163F4 (not REd as a
    // type -- do not call).
    virtual bool  GrantAlchemyReward(C_Soul* soul, const void* rewardDesc,
                                     CryGUID* outProductId, uint32_t* outCount) = 0;  // [13] +0x68  0x181FD43E0
    // AlchemyRecipeStepsTolerance, +AlchemyTrialEndErrorPerkTolerance with soul ability 19.
    virtual int   GetAllowedFailedSteps(C_Soul* soul) = 0;                    // [14] +0x70  0x182CF3AF8
    // Uncertified fillers -- DO NOT call through them.
    virtual void _vf15() = 0;                                                 // [15]
    virtual void _vf16() = 0;                                                 // [16]
    virtual void _vf17() = 0;                                                 // [17]
    virtual void _vf18() = 0;                                                 // [18]
    virtual void _vf19() = 0;                                                 // [19]
    virtual void _vf20() = 0;                                                 // [20]
    virtual void _vf21() = 0;                                                 // [21]
    virtual void _vf22() = 0;                                                 // [22]
    virtual void _vf23() = 0;                                                 // [23]
    virtual void _vf24() = 0;                                                 // [24]
    virtual void _vf25() = 0;                                                 // [25]
    virtual void _vf26() = 0;                                                 // [26]
    virtual void _vf27() = 0;                                                 // [27]
    virtual void _vf28() = 0;                                                 // [28]
    virtual void _vf29() = 0;                                                 // [29]
    virtual void _vf30() = 0;                                                 // [30]
    virtual void _vf31() = 0;                                                 // [31]
    virtual void _vf32() = 0;                                                 // [32]
    virtual void _vf33() = 0;                                                 // [33]
    virtual void _vf34() = 0;                                                 // [34]
    virtual void _vf35() = 0;                                                 // [35]
    // "Is this recipe DOCUMENT's content known?"  Takes a Document-class (E_ItemType 8) item
    // guid -- any other item type returns 0.  Order: the doc float map (>0 wins) else, per the
    // doc's alchemy subobject, the BS string path (soul HasPerk via the name-keyed recipe DB)
    // or IsRecipeLearned on the linked recipe id -> 1.0/0.0.  Consumers: the potion/document
    // list "unknown recipe" badge (C_UIFlashListItem ctor sub_1808F67C4 byte +0x55) and the
    // prefer-unknown-recipe-sheet picker sub_181FD25A0.  NOT a brewed-potion query -- the
    // brewed set is the UniquePotionsBrewed DistinctGuid statistic (C_Statistics.h).
    virtual float GetRecipeDocKnownMark(const CryGUID& recipeDocItemGuid) = 0; // [36] +0x120  0x18119F2D0
    // Slots [37..82] exist (83 total, last +0x290) -- unmodeled; extend only with certification.
};

}  // namespace wh::rpgmodule
