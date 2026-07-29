#pragma once

#include <cstdint>
#include <functional>
#include <random>
#include <vector>

#include "CryEngine/CryCommon/CryExtension/CryGUID.h"

namespace wh::rpgmodule { class C_Soul; }

namespace Autobrew {

// One ingredient row of the recipe requirements, folded out of the compiled step conditions by
// Executor::BuildPlan -- and mutated by MistakeEngine's slips before op emission.
struct S_IngredientPlan {
    CryGUID  guid;
    uint32_t qty;
    bool     milled;       // IsMilled(g,1) step present
    bool     mustNotMill;  // explicit IsMilled(g,0) step present (a failable step of its own --
                           //   grinding a row with NO IsMilled step fails nothing)
    int      weakTurns;    // WeakBoilingTime target; -1 = no condition (any accrual tolerated)
    int      strongTurns;  // StrongBoilingTime target; -1 = no condition
};

// Phase-2 balancing: deliberate, skill-scaled mistakes, so autobrew brews like Henry actually
// brews instead of like a machine.  The engine only mutates OUR plan inputs (parsed rows, boil
// window targets, finish/stage selection); the brew is then physically EXECUTED with the
// mistake and the game's own grader judges it -- quality bands, mistake barks, XP and the
// >4-failed-steps explosion are all native.  Nothing is faked and no game logic is
// reimplemented.  Model + math: analysis/alchemy_re/balancing_harness_research.md §3.
//
// Skill model (armed per batch by BeginBatch):
//   L      = soul->GetSkillFraction(6)       -- the alchemy fraction the game's own tolerance
//                                               lerp uses (I_RPGMinigames::GetBrewTolerance)
//   apq    = soul->GetPerkStatModifier(apq)  -- "Alchemy-process quality" perk stat (Secrets of
//                                               Equilibrium).  Proven NOT to feed the tolerance
//                                               band, so damping our rates by it is the perk's
//                                               steady-hands flavor, not a double-count.
//   S      = clamp01(L + apq)                -- steadiness
//   sigma  = tolBase * intensity * rate * (1 - clamp01(apq))   -- boil jitter width, anchored
//            to the level-0 tolerance so one-sigma misses are exactly what a fresh brewer fails
//   slipP  = 0.25 * intensity * rate * (1 - S) per candidate    (rate = 1.5 in Hardcore)
//   majorP = 0.15 * intensity * (1 - S), Hardcore only
// Boil pass/fail is NOT decided here: the window target is jittered and the game's skill-lerped
// band |1 - turns/N| <= GetBrewTolerance() judges it -- low skill fails what high skill
// forgives.  No cap on stacked failures (user decision): a lowest-skill brew of a complex
// recipe can genuinely explode.
class MistakeEngine {
public:
    // Arm the per-batch state: snapshot the MCM mode/intensity and the skill model, clear the
    // log.  Called once per batch from Tick's Arming leg, BEFORE StockStations (sloppy-dried
    // rolls happen there, the rest in BuildPlan).  Off-mode or a missing soul disarms.
    void BeginBatch(const wh::rpgmodule::C_Soul* soul);

    bool Active() const { return m_active; }

    // StockStations slip: "grabbed the dried jar though fresh sits right there" -- quality-only
    // drag (AlchemyDryHerbQuality), fails no step.  Call only when BOTH the fresh item and a
    // dried stack are actually in the inventory.
    bool RollSloppyDried(const CryGUID& freshClassGuid);

    // BuildPlan slips, before emission: mutates rows in place (skip-grind / wrong-grind /
    // over-count / under-count).  countAvailable(classGuid) = units the player holds of the
    // class incl. its dried variant -- the over-count guard (an extra take from an exhausted
    // stack risks wedging the driver into the stall abort).
    void RollSlips(std::vector<S_IngredientPlan>& rows,
                   const std::function<uint32_t(const CryGUID&)>& countAvailable);

    // Boil windows: TRUE target seconds in, jittered seconds out (>= 0, applied BEFORE the
    // stop-lead subtraction).  eps ~ N(0, sigma), clamped to +-2.5 sigma and +-100%.
    float PerturbWindowSeconds(float trueSeconds);

    // Hardcore majors (always false outside Hardcore).
    bool RollTempSwap();          // per single-mode boil stage: weak<->strong window swap
    bool RollSkipDistill();       // per brew: phial finish instead of the retort
    bool RollSkipPowderGrind();   // per brew: phial finish instead of the mortar finish

    // ONE "[Autobrew] mistakes ..." kcd.log line per batch (skill snapshot + every roll) --
    // keeps field reports diagnosable.  Called from BuildPlan's success tail.
    void FlushLog(uint32_t recipeId);

    static constexpr uint32_t kSkillIdAlchemy = 6;   // skill selector (perk.xml skill_selector)
    static constexpr float kSlipBaseProbability  = 0.25f;
    static constexpr float kMajorBaseProbability = 0.15f;
    static constexpr float kHardcoreRateMult     = 1.5f;
    static constexpr float kEpsilonClampSigmas   = 2.5f;

private:
    bool Roll(float p);
    void AppendLog(const char* fmt, ...);

    std::mt19937 m_rng{ std::random_device{}() };
    bool  m_active   = false;
    bool  m_hardcore = false;
    float m_sigma  = 0.0f;
    float m_slipP  = 0.0f;
    float m_majorP = 0.0f;
    char  m_log[448] = {};
    int   m_logLen = 0;
};

}  // namespace Autobrew
