#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "CryEngine/CryCommon/CryString.h"
#include "core/Mistakes.h"
#include "playermodule/E_AlchemyVerb.h"

namespace wh::playermodule {
class C_Alchemy;
class C_AlchemyRecipe;
class C_AlchemyResource;
class I_ActionSets;
}
namespace wh::entitymodule { class C_Item; }

namespace Autobrew {

// The auto-brew driver for ONE brewing session -- constructed by OnSessionStart (registers the
// action rows), destroyed by OnSessionEnd (unregisters them).
//
// ARCHITECTURE: a single per-frame Tick() (tasks/Tick/) is the whole state machine -- it is the
// only place game state is sampled, intents are consumed, phases transition and work is
// scheduled.  The action-row callbacks record an intent bool and nothing else (rows can
// dispatch even while greyed, so validation belongs to the tick, not the callback).  Everything
// Tick() calls is either a pure query or a worker that assumes its preconditions -- no
// scattered gate checks.
//
// Flow: Idle (book prompt lights while pressing would work) -> press arms the recipe and
// auto-closes the book (synthetic read_back) -> Arming waits for the table -> clean a dirty
// workspace via the game's own I_Minigame Reset() virtual (NEVER the ctor-only brew-state POD
// reset -- that zeroes the kind-entity ids and crashes the C_PotEffectAction ambient), THEN
// stock (Reset tail-calls ClearIngredientSlots, 0x1806C5096 -- stocking first gets wiped),
// build the plan -> Cooking steps it op by op -- verbs, pot position, closed-loop boil windows
// -- with a hold-"Stop" prompt.  A finished plan chains BACK to Arming with the same recipe:
// batches repeat until CanBrewFrom runs dry (or Stop / a stall).
//
// Result presentation: hooks/BrewResult/ intercepts the gated presenter (the
// `if (m_presentResult) emit m_onBrewResult` helper, GrantBrewReward's only outlet) -- batch
// verdicts with a potion become a center toast and the emit is skipped (no game-pausing reward
// modal); everything else runs vanilla.  GradeBrew's no-recipe fail emits directly and is
// deliberately unhooked: fail messages are already non-blocking.
//
// Action NAMES must exist in the loaded action-map profile and have defaultActionHelp.xml rows
// (both shipped in autobrew.pak; regenerate with analysis/autobrew_rebuild_configs.py -- never
// edit the overrides in place).  Full RE: analysis/alchemy_re/action_registration_re.md +
// ui_and_autobrew.md.
class Executor {
public:
    explicit Executor(wh::playermodule::C_Alchemy* alchemy);
    ~Executor();

    Executor(const Executor&) = delete;
    Executor& operator=(const Executor&) = delete;

    // Action-row callbacks: record the intent ONLY; Tick() validates and acts (all main-thread,
    // no atomics needed -- input dispatch and KCSE tasks share the frame).
    void OnAutobrewPressed() { m_autobrewPressed = true; }   // "Autobrew" press in the book
    void OnStopPressed()     { m_stopPressed = true; }       // "Stop" hold at the table

    void Tick();   // THE state machine (tasks/Tick/)

    // hooks/BrewResult/ seam: true = batch verdict with a potion, center toast shown -- the
    // caller skips the m_onBrewResult emit (= no reward modal).  Manual brews and item-less
    // verdicts return false and stay fully vanilla.
    bool HandleBrewResult(wh::playermodule::C_Alchemy* session, char code,
                          wh::entitymodule::C_Item* item, uint32_t count);

private:
    enum class E_Phase {
        Idle,      // session live; the book prompt may arm a brew
        Arming,    // recipe picked, book auto-closing; waiting for the table
        Cooking,   // plan executing op by op
    };

    struct S_PlanOp {
        enum E_Kind {
            DoVerb,          // dispatch op.verb once the director slot is free
            TakeIngredient,  // TakeHerbN for the station slot holding op.guid
            EnsureHoldPot,   // TakePot from Idle (distill / pour-phial prep)
            SetPotOnFire,    // swing the kettle halter until m_potOnFire (boil windows need it)
            TakePotOffFire,  // swing back: accrual stops the moment the swing starts
            BoilWeak,        // keep the pot on the fire until this stage ADDS targetSeconds of WEAK boil
            BoilStrong,      // same on the STRONG accumulator, bellows-maintained
        };
        E_Kind                                 kind;
        wh::playermodule::E_AlchemyVerb::Type  verb;           // DoVerb
        CryGUID                                guid;           // TakeIngredient
        float                                  targetSeconds;  // Boil*: seconds of boil this stage
                                                               //   must add (delta on the pot's
                                                               //   base-liquid record)
    };

    struct S_OpRuntime {               // per-op scratch -- reset wholesale (`m_op = {}`) on advance
        double stallStartedAt  = 0.0;   // ETIMER_GAME timestamp: continuous no-progress window
        double nextStallLogAt  = 0.0;   // absolute timestamp for the next wedge snapshot
        double boilStartedAt   = 0.0;   // hard-deadline clock for the current Boil* op
        float  boilBaseline    = 0.0f;  // stage zero: base-resource accumulator at op start
        bool   stallActive     = false;
        bool   boilTimerActive = false;
        bool   baselineTaken   = false;
    };

    static wh::playermodule::I_ActionSets* GetActionSets();
    static void ShowPrompt(const CryStringT<char>& context, const CryStringT<char>& action,
                           bool installed);
    
    
    static void ShowCenterToast(const CryStringT<char>& text);

    // ---- queries (pure reads) ----
    // The recipe currently open in the book's detail view (0 = none / page busy).
    uint32_t GetOpenRecipeId() const;
    // The pot bucket's base-liquid record (poured first, present for every boil window).
    const wh::playermodule::C_AlchemyResource* FindPotBaseRecord() const;
    // The dried-variant class guid for a fresh herb class (item.xml DriedItemId, live off the
    // item DB -- the game's own one-hop brewing substitute); null guid for anything else.
    static CryGUID FindDriedVariant(const CryGUID& classGuid);
    // The take-verb for whichever station slot is keyed to the ingredient class OR its dried
    // variant -- herb slots (TakeHerb1+i) AND special slots (TakeSpecial1+i, crushable specials
    // like pearl/salt/coal stock there) -- or None if no slot holds it.
    wh::playermodule::E_AlchemyVerb::Type FindIngredientVerb(const CryGUID& guid) const;
    // Any station bucket non-empty (leftovers of a previous brew).
    bool IsWorkspaceDirty() const;
    // The game's own save-persisted brewed-before flag (C_RPGMinigames product brewed mark,
    // any quality tier >= 1.0) -- the require-brewed MCM gate.  No bookkeeping of ours.
    static bool RecipeBrewedOnce(uint32_t recipeId);

    // ---- workers (no world-state gating; Tick establishes the preconditions) ----
    // Stock the herb/special stations from the player's inventory (autocook's exact path).
    bool StockStations(wh::playermodule::C_AlchemyRecipe* recipe);
    // Compile the recipe's game-compiled step conditions into m_plan (false = refuse).
    bool BuildPlan(uint32_t recipeId);
    void BeginCooking();   // Arming -> Cooking (shows the stop prompt)
    void ReturnToIdle();   // unified teardown: finish, cancel and stall-abort all land here
    void AbortRun(const char* reason);   // loud ReturnToIdle: kcd.log line + center toast

    static constexpr double kStallAbortSeconds      = 10.0; // continuous no-progress limit
    static constexpr double kStallLogIntervalSeconds = 2.0; // wedge snapshot cadence
    static constexpr double kBoilGraceSeconds        = 60.0; // beyond 2x the requested window

    // soul_ability__autobrew.xml row, granted by the "Routine" perk (perk__autobrew.xml,
    // alchemy tree).  Vanilla ability ids stop around ~130; >= 200 is mod space -- the
    // grant/lookup chain is validation-free (see rpgmodule/C_Soul.h S_SoulAbilityMap).
    static constexpr uint32_t kSoulAbilityAutobrewRoutine = 200;

    // Boil timing, in SECONDS.  Recipe turns are converted once in BuildPlan via the
    // HourglassTimeout RPG param (10 s -- the exact divisor of the game's boil sensor); progress
    // is the base record's accumulator delta.  Pass band = |1 - t/N| <= skill tolerance.
    static constexpr float kStopLeadSec          = 0.5f;   // swing-off dispatch latency (accrual
                                                           //   halts at the "moving" anim event)
    static constexpr float kPreStrongWeakLeadSec = 1.5f;   // the first bellows montage keeps
                                                           //   accruing WEAK until its squeeze event
    static constexpr float kSecondsPerIntensity  = 6.0f;   // X drains 1/6 per s (StrongBoilTimeout
                                                           //   10 table prop) down to the 1.0 floor,
                                                           //   so charge = (X - 1) * 6 s of strong

    wh::playermodule::C_Alchemy* m_pAlchemy;
    bool     m_registered = false;       // action rows are installed (ctor succeeded)

    E_Phase  m_phase = E_Phase::Idle;
    bool     m_autobrewPressed = false;  // intents: set by the callbacks, consumed by Tick
    bool     m_stopPressed = false;
    uint32_t m_pendingRecipeId = 0;      // Arming's payload

    std::vector<S_PlanOp> m_plan;
    size_t      m_planCursor = 0;
    S_OpRuntime m_op;
    MistakeEngine m_mistakes;   // Phase-2 balancing: per-batch skill-scaled mistake rolls
                                //   (armed in Arming, consumed by StockStations + BuildPlan)
};

inline std::unique_ptr<Executor> g_executor;

}  // namespace Autobrew
