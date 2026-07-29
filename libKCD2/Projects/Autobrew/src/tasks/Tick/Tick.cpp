#include "tasks/Tick/Tick.h"

#include "KCSE/KCSEAPI.h"

#include <cstring>

#include "crysystem/SSystemGlobalEnvironment.h"
#include "Offsets/vtables/ITimer.h"
#include "entitymodule/C_Actor.h"
#include "entitymodule/C_Inventory.h"
#include "playermodule/C_ActionSets.h"
#include "playermodule/I_ActionSets.h"
#include "playermodule/C_Alchemy.h"
#include "playermodule/C_AlchemyRecipeDatabase.h"
#include "playermodule/C_AlchemyResource.h"
#include "rpgmodule/C_Soul.h"

#include "core/Executor.h"
#include "mcm.h"

namespace Autobrew {

using namespace wh::playermodule;

void Tick()
{
    if (g_executor)
        g_executor->Tick();
    KCSE::GetTaskInterface()->AddTask(&Tick);
}

// The whole driver state machine.  This is the ONLY place game state is sampled, intents are
// consumed and phases transition; everything called from here is a pure query or a gate-free
// worker.
void Executor::Tick()
{
    auto* alc = m_pAlchemy;
    auto& bs = alc->m_brewState;
    auto* env = SSystemGlobalEnvironment::GetInstance();
    const bool haveGameClock = env && env->pTimer;
    const double now = haveGameClock
        ? env->pTimer->GetCurrTimeD(Offsets::ITimer::ETIMER_GAME)
        : 0.0;

    // ---- world snapshot ----
    const char* context   = alc->m_currentContext.c_str();
    const bool inReading  = std::strcmp(context, "alchemy_reading") == 0;
    const bool atTable    = std::strcmp(context, "alchemy") == 0;
    const bool brewing    = bs.m_mode == E_AlchemyMode::Brewing;
    const bool bookLanded = bs.m_mode == E_AlchemyMode::Reading;   // open montage done -- the
                                                                   //   vanilla light-up instant
    const bool verbBusy   = alc->m_director.IsSlotOccupied(E_AlchemyDirectorSlot::Verb);
    const bool tableIdle  = alc->GetEffectiveState() == E_AlchemyState::Idle;
    const uint32_t openRecipeId = GetOpenRecipeId();

    // ---- intents (recorded by the action-row callbacks between our frames) ----
    const bool pressedAutobrew = m_autobrewPressed;
    const bool pressedStop     = m_stopPressed;
    m_autobrewPressed = false;
    m_stopPressed = false;

    // ---- book prompt: lit only while pressing would work (vanilla read_* timing) ----
    // MCM main switch folds into visibility AND the light (all three row properties false/empty
    // = the helpbar uninstalls the row entirely).
    // The Routine perk wall folds into VISIBILITY: without the perk the feature hides
    // outright (no greyed teaser).  Possession = one sorted-map probe on the soul.
    bool perkOk = true;
    if (g_requirePerk) {
        auto* actor = alc->m_pPlayerActor;
        auto* soul = actor ? actor->m_pSoul : nullptr;
        perkOk = soul && soul->HasAbility(kSoulAbilityAutobrewRoutine);
    }
    // The manual-brew wall GREYS per recipe (unlike the perk wall): the prompt stays visible
    // on unbrewed pages but unlit, with a "brew it by hand once" reason -- same presentation
    // as the vanilla missing-ingredients grey-out.  Flag = the game's own brewed record
    // (see RecipeBrewedOnce).
    bool brewedOk = true;
    if (g_requireBrewed && openRecipeId != 0)
        brewedOk = RecipeBrewedOnce(openRecipeId);
    bool lit = g_enableAutobrew && perkOk && brewedOk && m_phase == E_Phase::Idle && inReading
               && bookLanded && openRecipeId != 0;
    bool missingIngredients = false;
    if (lit) {
        // Availability folds into the light, exactly like vanilla greys read_select when
        // ingredients are missing.  Only evaluated while a recipe page is up.
        auto* recipe = C_AlchemyRecipeDatabase::GetRecipeById(openRecipeId);
        auto* actor = alc->m_pPlayerActor;
        auto* inventory = actor ? actor->GetInventory() : nullptr;
        lit = recipe && inventory && recipe->CanBrewFrom(inventory);
        missingIngredients = !lit;
    }

    GetActionSets()->SetActionVisible("alchemy_reading", "alch_autobrew",
                                      g_enableAutobrew && perkOk, 1);

    GetActionSets()->SetActionEnabled("alchemy_reading", "alch_autobrew", lit, 1);

    const char* disableReason = "";
    if (g_enableAutobrew && perkOk) {
        if (!brewedOk)
            disableReason = "ui_autobrew_needs_brew";       // shipped in the Autobrew loc paks
        else if (missingIngredients)
            disableReason = "dlg_alch_not_all_ingredients";
    }
    GetActionSets()->SetActionDisableReason("alchemy_reading", "alch_autobrew",
                                            disableReason, 1);

    // ---- stop cancels any phase (the row dispatches even while hidden) ----
    // The MCM main switch is a START gate only (folded into `lit`): flipping it off
    // mid-run lets the running batch finish on its own.
    if (pressedStop && m_phase != E_Phase::Idle) {
        ReturnToIdle();
        return;
    }

    switch (m_phase) {
    case E_Phase::Idle: {
        if (!pressedAutobrew || !lit)
            break;   // lit already embeds page validity AND ingredient availability
        m_pendingRecipeId = openRecipeId;
        m_phase = E_Phase::Arming;
        // Auto-close the book.  Dispatching from the tick task is the same execution context
        // the deferred read_back task used -- outside any action-row invoke.
        static_cast<C_ActionSets*>(GetActionSets())->TriggerAction("read_back");
        break;
    }
    case E_Phase::Arming: {
        // verbBusy for blocking reset while pouring into phial
        if (!atTable || !brewing || !tableIdle || verbBusy)
            break;   // book closing / walking back / finish flow of the previous batch
        
        if (IsWorkspaceDirty())
            alc->Reset();
        

        auto* recipe = C_AlchemyRecipeDatabase::GetRecipeById(m_pendingRecipeId);
        auto* actor = alc->m_pPlayerActor;
        auto* inventory = actor ? actor->GetInventory() : nullptr;
        const char* refusal = nullptr;
        if (!recipe || !inventory)
            refusal = "session lost the recipe or inventory";
        else if (!recipe->CanBrewFrom(inventory))
            refusal = "ingredients ran out";   // the normal end of a batch loop
        else {
            // Arm the per-batch mistake rolls (skill snapshot + fresh RNG draws each batch):
            // StockStations rolls the sloppy-dried pick, BuildPlan the rest.
            m_mistakes.BeginBatch(actor->m_pSoul);
            if (!StockStations(recipe))
                refusal = "ingredient stocking failed";
            else if (!BuildPlan(m_pendingRecipeId))
                refusal = "recipe not auto-brewable";
        }
        if (refusal)
            AbortRun(refusal);
        else
            BeginCooking();
        break;
    }
    case E_Phase::Cooking: {
        if (m_planCursor >= m_plan.size()) {
            // Brew complete -> chain the next batch: back to Arming with the SAME recipe.
            // Its gate waits out the finish montage (grade+Reset run inside it, leaving a
            // clean Idle workspace) and re-stocks; when the ingredients run out the arming
            // check fails and ReturnToIdle ends the loop (restoring the result popup).
            // Looping off = single brew: back to Idle BEFORE the verdict lands, so
            // HandleBrewResult passes it through and the vanilla result popup shows.
            if (g_enableLooping)
                m_phase = E_Phase::Arming;
            else
                ReturnToIdle();
            break;
        }
        const S_PlanOp& op = m_plan[m_planCursor];
        // Boil ops keep running DURING verb montages: they only monitor accrual and feed
        // the input buffer (the continuous-bellows chain depends on it); every dispatching
        // op still waits the Verb slot out.
        const bool boilOp = op.kind == S_PlanOp::BoilWeak || op.kind == S_PlanOp::BoilStrong;

        const auto clearStall = [&]() {
            m_op.stallActive = false;
            m_op.stallStartedAt = 0.0;
            m_op.nextStallLogAt = 0.0;
        };
        const auto markStall = [&]() {
            if (!haveGameClock)
                return;
            // ResetTimer cannot normally occur during a live brew, but restarting the window is
            // safer than interpreting a backwards clock jump as an enormous elapsed duration.
            if (!m_op.stallActive || now < m_op.stallStartedAt) {
                m_op.stallActive = true;
                m_op.stallStartedAt = now;
                m_op.nextStallLogAt = now + kStallLogIntervalSeconds;
            }
        };
        const auto stallElapsed = [&]() {
            return m_op.stallActive && now >= m_op.stallStartedAt
                ? now - m_op.stallStartedAt
                : 0.0;
        };
        const auto logStall = [&]() {
            if (!m_op.stallActive || now < m_op.nextStallLogAt)
                return;
            if (env && env->pLog) {
                env->pLog->LogAlways(
                    "[Autobrew] stalled %.2f seconds: op %zu/%zu kind %d verb %d canPerform %d "
                    "state %d verbBusy %d potFire %d potMoving %d X %.2f pending %d mode %d",
                    stallElapsed(), m_planCursor, m_plan.size(), int(op.kind), int(op.verb),
                    int(alc->CanPerformVerb(op.verb)), int(alc->GetEffectiveState()),
                    int(verbBusy), int(bs.m_potOnFire), int(bs.m_potMoving),
                    bs.m_fireIntensity, int(alc->m_pendingVerb), int(bs.m_mode));
            }
            // Do not emit a burst of catch-up lines after one unusually long frame.
            m_op.nextStallLogAt = now + kStallLogIntervalSeconds;
        };

        // Director occupancy means the previously dispatched native action is still valid.  In
        // particular C_UseMortarAction temporarily changes mode Brewing -> Grinding -> Brewing;
        // only judge a non-Brewing mode after that action has actually left the Verb slot.
        if (verbBusy && !boilOp) {
            clearStall();
            break;
        }
        if (!brewing) {
            markStall();
            logStall();
            if (stallElapsed() >= kStallAbortSeconds)
                AbortRun("brewing was interrupted");
            break;
        }
        {
            bool advanced = false;
            bool progressed = false;
            bool boilTimedOut = false;
            switch (op.kind) {
            case S_PlanOp::DoVerb:
                if (alc->CanPerformVerb(op.verb)) {
                    alc->PerformVerb(op.verb);
                    advanced = true;
                }
                break;
            case S_PlanOp::TakeIngredient: {
                const auto verb = FindIngredientVerb(op.guid);
                if (verb != E_AlchemyVerb::None && alc->CanPerformVerb(verb)) {
                    alc->PerformVerb(verb);
                    advanced = true;
                }
                break;
            }
            case S_PlanOp::EnsureHoldPot: {
                const auto st = alc->GetEffectiveState();
                if (st == E_AlchemyState::HoldingPot) {
                    advanced = true;
                } else if (st == E_AlchemyState::Idle
                           && alc->CanPerformVerb(E_AlchemyVerb::TakePot)) {
                    alc->PerformVerb(E_AlchemyVerb::TakePot);
                    advanced = true;
                }
                break;
            }
            case S_PlanOp::SetPotOnFire:
            case S_PlanOp::TakePotOffFire: {
                const bool wantFire = op.kind == S_PlanOp::SetPotOnFire;
                if (bs.m_potMoving)
                    break;   // swing in flight; m_potOnFire flips on its "done" anim event
                if (bs.m_potOnFire == wantFire) {
                    advanced = true;
                    break;
                }
                // Verb 20 is the halter-toggle input (delegate #6) -- position-agnostic, one
                // swing per dispatch.  Advance only once the position byte reads back.
                if (alc->CanPerformVerb(E_AlchemyVerb::MovePotBack)) {
                    alc->PerformVerb(E_AlchemyVerb::MovePotBack);
                    progressed = true;
                }
                break;
            }
            case S_PlanOp::BoilWeak:
            case S_PlanOp::BoilStrong: {
                const bool strong = op.kind == S_PlanOp::BoilStrong;
                // Progress in raw SECONDS off the pot's base-liquid record: the accrual writer
                // adds the same dt to EVERY pot record, so the base's delta since this op
                // started IS the boil time this stage has added.
                const C_AlchemyResource* base = FindPotBaseRecord();
                if (!base)
                    break;   // no base liquid in the pot -> wedged; the stall abort takes over
                const float accrued = strong ? base->m_strongBoilSeconds : base->m_weakBoilSeconds;
                if (!m_op.baselineTaken) {
                    m_op.baselineTaken = true;
                    m_op.boilBaseline = accrued;
                }
                if (haveGameClock
                    && (!m_op.boilTimerActive || now < m_op.boilStartedAt)) {
                    m_op.boilTimerActive = true;
                    m_op.boilStartedAt = now;
                }
                if (m_op.boilTimerActive
                    && now - m_op.boilStartedAt
                           >= 2.0 * static_cast<double>(op.targetSeconds) + kBoilGraceSeconds) {
                    boilTimedOut = true;
                    break;
                }
                if (accrued - m_op.boilBaseline >= op.targetSeconds) {
                    if (strong && alc->m_pendingVerb == E_AlchemyVerb::UseBellows)
                        alc->m_pendingVerb = E_AlchemyVerb::None;   // un-press: no trailing pull
                    advanced = true;
                    break;
                }
                if (!bs.m_potOnFire && !bs.m_potMoving)
                    break;   // pot left the fire mid-window -> wedged; the stall abort takes over
                

                if (strong) {
                    const float remaining = op.targetSeconds - (accrued - m_op.boilBaseline);
                    const float charge = (bs.m_fireIntensity - 1.0f) * kSecondsPerIntensity;
                    if (remaining > charge)
                        alc->m_pendingVerb = E_AlchemyVerb::UseBellows;
                    else if (alc->m_pendingVerb == E_AlchemyVerb::UseBellows)
                        alc->m_pendingVerb = E_AlchemyVerb::None;
                }
                progressed = true;   // waiting on the resource's game-time accrual is progress
                break;
            }
            }
            if (boilTimedOut) {
                AbortRun("boiling timed out");
                break;
            }
            if (advanced) {
                ++m_planCursor;
                m_op = {};
            } else if (progressed) {
                clearStall();
            } else {
                markStall();
            }
        }
        // Wedge diagnostics use the same game clock as the abort, so their cadence is stable at
        // every frame rate and pauses/time scaling track the native alchemy actions.
        logStall();
        if (stallElapsed() >= kStallAbortSeconds)
            AbortRun("stalled");   // the 2 s snapshots above pin the op and failing gate
        break;
    }
    }
}

}  // namespace Autobrew
