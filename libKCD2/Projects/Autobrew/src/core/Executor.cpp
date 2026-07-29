#include "core/Executor.h"

#include <algorithm>
#include <cstdio>
#include <cstring>

#include "crysystem/CCryAction.h"
#include "entitymodule/C_Actor.h"
#include "entitymodule/C_DocumentItem.h"
#include "entitymodule/C_Inventory.h"
#include "entitymodule/C_Item.h"
#include "entitymodule/C_ItemDatabase.h"
#include "entitymodule/E_ItemType.h"
#include "entitymodule/S_HerbItemClass.h"
#include "entitymodule/S_ItemClass.h"
#include "framework/C_EEExpression.h"
#include "framework/C_ExpressionEngine.h"
#include "framework/GuidUtils.h"
#include "game/S_GameContext.h"
#include "playermodule/C_PlayerModule.h"
#include "playermodule/I_ActionSets.h"
#include "playermodule/C_Alchemy.h"
#include "playermodule/C_AlchemyRecipeDatabase.h"
#include "playermodule/C_AlchemyResource.h"
#include "rpgmodule/C_Statistics.h"
#include "rpgmodule/S_RpgParams.h"
#include "guimodule/ItemUiPresentation.h"
#include "crysystem/SSystemGlobalEnvironment.h"

namespace Autobrew {

using namespace wh::playermodule;

Executor::Executor(C_Alchemy* alchemy)
    : m_pAlchemy(alchemy)
{
    auto* as = GetActionSets();
    if (!as)
        return;   // no action registry (never seen mid-session) -- stay inert

    // Labels/rings come from the defaultActionHelp.xml rows (shipped in autobrew.pak).
    // "Autobrew" lives in the BOOK (alchemy_reading); "Stop" at the table (alchemy).
    // Callbacks only record intents -- Tick() owns every decision.
    as->RegisterAction("alchemy_reading", "alch_autobrew",
                       [] { if (g_executor) g_executor->OnAutobrewPressed(); }, 1);
    as->RegisterAction("alchemy", "alch_autobrew_stop",
                       [] { if (g_executor) g_executor->OnStopPressed(); }, 1);

    // Reading prompt: VISIBLE from the start but greyed (enabled=false), like the vanilla
    // read_* rows during the turn-to-book transition -- Tick lights it only while pressing
    // would actually work.
    as->SetActionVisible("alchemy_reading", "alch_autobrew", 1, 1);
    as->SetActionEnabled("alchemy_reading", "alch_autobrew", false, 1);
    ShowPrompt("alchemy", "alch_autobrew_stop", false);

    m_registered = true;
}

Executor::~Executor()
{
    if (!m_registered)
        return;

    if (auto* as = GetActionSets()) {
        as->UnregisterAction("alchemy_reading", "alch_autobrew", 1);
        as->UnregisterAction("alchemy", "alch_autobrew_stop", 1);
    }
}

wh::playermodule::I_ActionSets* Executor::GetActionSets()
{
    auto* ctx = wh::game::S_GameContext::GetInstance();
    auto* pm = ctx ? ctx->m_pPlayerModule : nullptr;
    return pm ? pm->m_pActionSets : nullptr;
}

void Executor::ShowPrompt(const CryStringT<char>& context, const CryStringT<char>& action,
                          bool installed)
{
    GetActionSets()->SetActionEnabled(context, action, installed, 1);
    GetActionSets()->SetActionVisible(context, action, installed, 1);
}

// The recipe currently open in the book's detail view, resolved exactly like the native
// autocook gate: book prop entity (m_kindEntityId[OpenBook]) -> IItemSystem::GetItem ->
// C_DocumentItem, valid only while actually reading a recipe page.
uint32_t Executor::GetOpenRecipeId() const
{
    const EntityId bookEntityId =
        EntityId(m_pAlchemy->m_brewState.m_kindEntityId[E_AlchemyVerb::OpenBook]);
    if (!bookEntityId)
        return 0;

    auto* fw = CCryAction::GetInstance();
    auto* itemSystem = fw ? fw->GetIItemSystem() : nullptr;
    auto* book = reinterpret_cast<wh::entitymodule::C_DocumentItem*>(
        itemSystem ? itemSystem->GetItem(bookEntityId) : nullptr);
    if (!book)
        return 0;

    const bool onDetailPage = book->m_viewMode == wh::entitymodule::E_DocumentViewMode::RecipeDetail;
    const bool herbViewOpen = book->m_herbViewActive;
    if (!onDetailPage || herbViewOpen)
        return 0;
    return book->m_lastOpenRecipeId;
}

// The game's own brewed-before record: the UniquePotionsBrewed statistic (type DistinctGuid)
// keeps the save-persisted SET of every product guid ever brewed, so pre-mod brews count.  Per
// PRODUCT guid (each quality tier has its own), hence any-tier.  Successful autobrews land in
// the set too, but under the require-brewed gate none can run before the first manual brew.
// NOT C_RPGMinigames::IsRecipeLearned (reading the recipe document sets that) and NOT its
// vf+0x120 mark (a recipe-DOCUMENT knowledge query -- always 0 for potion guids).
bool Executor::RecipeBrewedOnce(uint32_t recipeId)
{
    auto* recipe = C_AlchemyRecipeDatabase::GetRecipeById(recipeId);
    auto* stats = wh::rpgmodule::C_Statistics::GetInstance();
    if (!recipe || !stats)
        return false;
    for (const auto& product : recipe->m_products)
        if (stats->HasDistinctGuid("UniquePotionsBrewed", product.m_productItemId))
            return true;
    return false;
}

const C_AlchemyResource* Executor::FindPotBaseRecord() const
{
    for (auto* rec : m_pAlchemy->m_brewState.m_buckets[E_AlchemyStationKind::Pot])
        if (rec && rec->m_isBase)
            return rec;
    return nullptr;
}

// The game's own one-hop brewing substitute (ingredient counter 0x18175A4BC / autocook retry
// 0x181FFE040): a fresh herb class resolves to its item.xml DriedItemId, anything else to the
// null guid.  Live off the item DB, so mod-added herbs resolve too.
CryGUID Executor::FindDriedVariant(const CryGUID& classGuid)
{
    using namespace wh::entitymodule;
    auto* classDef = C_ItemDatabase::GetInstance()->FindClassByGuid(classGuid);
    if (!classDef || !classDef->IsType(E_ItemType::Herb))
        return CryGUID{};
    return classDef->GetAsHerbItemClass()->m_driedItemId;
}

E_AlchemyVerb::Type Executor::FindIngredientVerb(const CryGUID& guid) const
{
    const auto& bs = m_pAlchemy->m_brewState;
    const CryGUID dried = FindDriedVariant(guid);
    const bool hasDried = dried != CryGUID{};
    for (int i = 0; i < 3; ++i)
        if (bs.m_herbSlotKey[i] == guid || (hasDried && bs.m_herbSlotKey[i] == dried))
            return E_AlchemyVerb::Type(E_AlchemyVerb::TakeHerb1 + i);
    for (int i = 0; i < 3; ++i)
        if (bs.m_specialSlotKey[i] == guid)
            return E_AlchemyVerb::Type(E_AlchemyVerb::TakeSpecial1 + i);
    return E_AlchemyVerb::None;
}

bool Executor::IsWorkspaceDirty() const
{
    for (const auto& bucket : m_pAlchemy->m_brewState.m_buckets)
        if (!bucket.empty())
            return true;
    return false;
}

bool Executor::StockStations(wh::playermodule::C_AlchemyRecipe* recipe)
{
    auto* actor = m_pAlchemy->m_pPlayerActor;
    auto* inventory = actor ? actor->GetInventory() : nullptr;
    if (!inventory)
        return false;

    std::vector<wh::framework::WUID> stock;
    for (auto& row : recipe->m_ingredients) {
        auto* item = inventory->FindItemByClass(row.m_ingredientItemId);
        if (item) {
            // Sloppy-dried slip: fresh is right there, but a careless brewer grabs the dried
            // jar (quality 0.3 vs 1.0 -- drags the grade's ingredient term, fails no step).
            // Rolled only when a dried stack actually exists.
            const CryGUID dried = FindDriedVariant(row.m_ingredientItemId);
            if (dried != CryGUID{})
                if (auto* driedItem = inventory->FindItemByClass(dried))
                    if (m_mistakes.RollSloppyDried(row.m_ingredientItemId))
                        item = driedItem;
        } else {
            const CryGUID dried = FindDriedVariant(row.m_ingredientItemId);
            if (dried != CryGUID{})
                item = inventory->FindItemByClass(dried);
        }
        if (!item)
            return false;   // ingredient missing after all -> refuse the brew
        stock.push_back(item->m_wuid);
    }
    m_pAlchemy->ClearIngredientSlots();
    for (auto& wuid : stock)
        m_pAlchemy->ApplyIngredient(wuid);
    return true;
}

// Compile the recipe into a verb sequence, folding the requirements out of each step's
// GAME-COMPILED Condition (C_AlchemyRecipeStep::m_pCompiledCondition) instead of parsing the
// text: the tokenizer pushes call arguments before the Call entry and folds constants, so a
// flat walk over the operand entries recovers every builtin with its exact literal args
// (string args = interned index into the EE engine's string pool).  Returns false -- leaving
// m_plan untouched -- for an unknown recipe, a condition the game itself couldn't compile,
// an unknown/undrivable builtin, or a GUID that matches no ingredient row: refuse rather
// than brew garbage.
bool Executor::BuildPlan(uint32_t recipeId)
{
    auto* recipe = C_AlchemyRecipeDatabase::GetRecipeById(recipeId);
    if (!recipe)
        return false;

    auto* ctx = wh::game::S_GameContext::GetInstance();
    auto* engine = ctx ? ctx->m_pExpressionEngine : nullptr;
    if (!engine)
        return false;

    std::vector<S_IngredientPlan> ing;
    for (auto& row : recipe->m_ingredients)
        ing.push_back({ row.m_ingredientItemId, row.m_quantity ? row.m_quantity : 1u,
                        false, false, -1, -1 });

    auto find = [&](const CryGUID& g) -> S_IngredientPlan* {
        for (auto& e : ing)
            if (e.guid == g)
                return &e;
        return nullptr;
    };
    // Resolve a string-literal call arg (pool index) to the ingredient row it names.
    auto findByArg = [&](float poolIndex) -> S_IngredientPlan* {
        const char* text = engine->GetPoolString(static_cast<int32_t>(poolIndex));
        CryGUID g;
        return (text && wh::ParseGuid(text, g)) ? find(g) : nullptr;
    };

    using S_Operand = wh::framework::C_EEExpression::S_Operand;

    bool distill = false;
    bool resultMilled = false;
    std::vector<float> lits;   // pending literal operands (call args, comparands)
    for (auto& step : recipe->m_steps) {
        const auto* expr = step.m_pCompiledCondition;
        if (!expr || expr->m_status != wh::framework::E_EEExpressionStatus::Resolved)
            return false;

        lits.clear();
        for (int32_t i = 0; i < expr->m_operandCount; ++i) {
            const S_Operand& opnd = expr->m_operands[i];
            if (opnd.m_type == S_Operand::FloatLiteral) {
                lits.push_back(opnd.m_value);
                continue;
            }
            if (opnd.m_type != S_Operand::Call)   // ctx-calls/variables never appear in recipes
                return false;

            const char* name = opnd.m_pFunction->GetName();
            const int argc = opnd.m_pFunction->GetArgCount();
            if (argc < 0 || static_cast<size_t>(argc) > lits.size())
                return false;
            const float* args = lits.data() + (lits.size() - argc);

            if (std::strcmp(name, "PotionBase") == 0 && argc == 0) {
                // the recipe's base is always poured first -- nothing to fold
            } else if (std::strcmp(name, "IngredientCount") == 0 && argc == 1) {
                if (!findByArg(args[0]))   // row quantities satisfy the >=N comparand
                    return false;
            } else if (std::strcmp(name, "IsMilled") == 0 && argc == 2) {
                auto* e = findByArg(args[0]);
                if (!e)
                    return false;
                if (args[1] == 1.0f)
                    e->milled = true;
                else
                    e->mustNotMill = true;   // explicit must-NOT-mill step: the wrong-grind
                                             //   slip's only valid target (its own step fails)
            } else if (std::strcmp(name, "WeakBoilingTime") == 0 && argc == 2) {
                auto* e = findByArg(args[0]);
                if (!e)
                    return false;
                e->weakTurns = std::max(e->weakTurns, static_cast<int>(args[1]));
            } else if (std::strcmp(name, "StrongBoilingTime") == 0 && argc == 2) {
                auto* e = findByArg(args[0]);
                if (!e)
                    return false;
                e->strongTurns = std::max(e->strongTurns, static_cast<int>(args[1]));
            } else if (std::strcmp(name, "IsBaseDistilled") == 0 && argc == 1) {
                if (args[0] == 1.0f)
                    distill = true;
            } else if (std::strcmp(name, "IsResultMilled") == 0 && argc == 1) {
                if (args[0] == 1.0f)
                    resultMilled = true;
            } else {
                return false;   // unknown builtin or unexpected arity
            }

            lits.resize(lits.size() - argc);
        }
    }

    // Boil scheduling.  Sort longest-first, then
    // verify the coupled constraints are schedulable at all -- refuse rather than mis-boil.
    std::stable_sort(ing.begin(), ing.end(),
                     [](const S_IngredientPlan& a, const S_IngredientPlan& b) {
                         if (a.weakTurns != b.weakTurns)
                             return a.weakTurns > b.weakTurns;
                         return a.strongTurns > b.strongTurns;
                     });
    {
        int needW = 0, needS = 0;   // reverse pass: a constrained target may never grow
        for (size_t i = ing.size(); i-- > 0;) {
            if (ing[i].weakTurns >= 0) {
                if (ing[i].weakTurns < needW)
                    return false;
                needW = ing[i].weakTurns;
            }
            if (ing[i].strongTurns >= 0) {
                if (ing[i].strongTurns < needS)
                    return false;
                needS = ing[i].strongTurns;
            }
        }
    }
    // Unconstrained slots inherit the next constrained target (any accrual is fine for them),
    // so windows only open at constrained anchors: window_i = eff_i - eff_{i+1}.
    std::vector<float> effW(ing.size()), effS(ing.size());
    {
        int w = 0, s = 0;
        for (size_t i = ing.size(); i-- > 0;) {
            if (ing[i].weakTurns >= 0)
                w = ing[i].weakTurns;
            if (ing[i].strongTurns >= 0)
                s = ing[i].strongTurns;
            effW[i] = float(w);
            effS[i] = float(s);
        }
    }

    // Deliberate mistakes, part 1 (see core/Mistakes.h; armed per batch in Tick's Arming leg):
    // the discrete slips mutate the parsed rows BEFORE emission -- grind flags and quantities
    // only, so the boil sort/feasibility above stay valid.  The boil-window jitter and the
    // Hardcore majors apply inline in the emission below.  countAvailable guards the
    // over-count slip: it needs a spare unit in the inventory or the extra take would run the
    // station stack dry and wedge the driver into the stall abort.
    {
        auto* actor = m_pAlchemy->m_pPlayerActor;
        auto* inventory = actor ? actor->GetInventory() : nullptr;
        m_mistakes.RollSlips(ing, [&](const CryGUID& g) -> uint32_t {
            if (!inventory)
                return 0;
            int32_t total = 0;
            if (auto* item = inventory->FindItemByClass(g))
                total += std::max(item->m_amount, 0);
            const CryGUID dried = FindDriedVariant(g);
            if (dried != CryGUID{})
                if (auto* item = inventory->FindItemByClass(dried))
                    total += std::max(item->m_amount, 0);
            return static_cast<uint32_t>(total);
        });
    }

    // Turns -> SECONDS, once, here: one sandglass turn = the HourglassTimeout RPG param (10 s),
    // the exact divisor of the game's boil sensor (0x182E2C110 reads S_Constants base+0x5F4).
    // The executor then compares raw record accumulators -- no per-frame back-conversion.
    float unit = wh::rpgmodule::S_RpgParamDefs::Get()->HourglassTimeout.Value().asFloat;
    if (unit <= 0.0f)
        unit = 1.0f;   // the sensor's own degenerate-value guard

    std::vector<S_PlanOp> plan;
    plan.push_back({ S_PlanOp::DoVerb,
                     E_AlchemyVerb::Type(E_AlchemyVerb::PourSpiritus + recipe->m_baseMaterial), {} });

    for (size_t i = 0; i < ing.size(); ++i) {
        const auto& e = ing[i];
        for (uint32_t u = 0; u < e.qty; ++u) {
            plan.push_back({ S_PlanOp::TakeIngredient, E_AlchemyVerb::None, e.guid });
            plan.push_back({ S_PlanOp::DoVerb,
                             e.milled ? E_AlchemyVerb::UseMortar : E_AlchemyVerb::UsePot, {} });
        }
        if (e.milled) {
            plan.push_back({ S_PlanOp::DoVerb, E_AlchemyVerb::UseMortar, {} });   // grind
            plan.push_back({ S_PlanOp::DoVerb, E_AlchemyVerb::UsePot, {} });      // dump mortar into pot
        }
        // This stage's boil windows.  Weak first: it self-sustains at the boil point with zero
        // strong bleed; then the bellows phase, and the lift the instant the strong target lands
        // (accrual halts on swing start, the leftover X drains harmlessly with the pot up).
        // The mistake jitter perturbs the TRUE target before the stop-leads come off: the
        // driver then honestly aims at the wrong time and the game's skill-lerped band judges
        // it.  A window jittered to zero simply never opens (a full undershoot).
        const float wSec = m_mistakes.PerturbWindowSeconds(
            (effW[i] - (i + 1 < ing.size() ? effW[i + 1] : 0.0f)) * unit);
        const float sSec = m_mistakes.PerturbWindowSeconds(
            (effS[i] - (i + 1 < ing.size() ? effS[i + 1] : 0.0f)) * unit);
        if (wSec > 0.0f || sSec > 0.0f) {
            // Hardcore temperature blunder: boil a single-mode window at the WRONG heat.  Only
            // stages with exactly one mode are swappable -- a dual window's weak-then-strong
            // lead choreography wouldn't survive the swap.
            const bool tempSwap = ((wSec > 0.0f) != (sSec > 0.0f)) && m_mistakes.RollTempSwap();
            plan.push_back({ S_PlanOp::SetPotOnFire, E_AlchemyVerb::None, {} });
            if (wSec > 0.0f)
                plan.push_back({ tempSwap ? S_PlanOp::BoilStrong : S_PlanOp::BoilWeak,
                                 E_AlchemyVerb::None, {},
                                 std::max(0.0f, wSec - (sSec > 0.0f ? kPreStrongWeakLeadSec
                                                                    : kStopLeadSec)) });
            if (sSec > 0.0f)
                plan.push_back({ tempSwap ? S_PlanOp::BoilWeak : S_PlanOp::BoilStrong,
                                 E_AlchemyVerb::None, {},
                                 std::max(0.0f, sSec - kStopLeadSec) });
            plan.push_back({ S_PlanOp::TakePotOffFire, E_AlchemyVerb::None, {} });
        }
    }

    // The finish differs by recipe type, and each finish flow grades + Resets BY ITSELF -- the
    // plan must END on the finishing verb (anything after it would run against the freshly
    // reset workspace and stall out the batch loop):
    //  - distilled: the Distill leg IS the finish.  C_UseRetortAction runs its own state
    //    machine (mode := Distilling, N bellow-distillation montage loops, then
    //    OnFinishRetortPotion grades + Resets).  Arming's mode/verbBusy gate waits it out.
    //  - result-milled (shot/gunpowder): UseMortar from Idle with the mortar EMPTY (every prep
    //    grind above ends by dumping into the pot) and >= 2 records in the pot routes to
    //    C_FinishRecipeFromMortarAction -- the whole pour-grind-sack montage;
    //    completion grades the MORTAR bucket, where the finish grind's mill bit
    //    satisfies IsResultMilled.  The pot must stay hanging: holding it leaves Idle and the
    //    verb would re-route to the plain mill action.
    //  - otherwise: pour the pot into the phial (C_UsePotAction's finish montage).
    if (distill && resultMilled)
        return false;   // never shipped together; the finish flows are mutually exclusive
    // Hardcore finishing blunders: fall back to the plain phial finish, so the recipe's
    // IsBaseDistilled / IsResultMilled step fails at the grade (barks 8 / 16).  Both finishes
    // stay physically valid -- the phial pour only needs the held pot with >= 2 records.
    if (distill && m_mistakes.RollSkipDistill())
        distill = false;
    if (resultMilled && m_mistakes.RollSkipPowderGrind())
        resultMilled = false;
    if (resultMilled) {
        plan.push_back({ S_PlanOp::DoVerb, E_AlchemyVerb::UseMortar, {} });
    } else {
        plan.push_back({ S_PlanOp::EnsureHoldPot, E_AlchemyVerb::None, {} });
        plan.push_back({ S_PlanOp::DoVerb,
                         distill ? E_AlchemyVerb::Distill : E_AlchemyVerb::UsePot, {} });
    }

    m_mistakes.FlushLog(recipeId);   // the one-line roll record; refusals above skip it
    m_plan = std::move(plan);
    return true;
}

void Executor::ShowCenterToast(const CryStringT<char>& text)
{
    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env || !env->pFlashUI)
        return;

    _smart_ptr<Offsets::IUIElement> elem;
    env->pFlashUI->GetUIElement(elem, "hud");
    if (!elem)
        return;
    auto fp = elem->GetFlashPlayer();
    if (!fp)
        return;

    Offsets::FlashVarPtr root;
    if (!fp->GetVariable("_root", root.put()) || !root)
        return;
    SFlashVarValue args[4] = { text.c_str(), 2, 3000, true };
    root->Invoke("fc_showInfoText", args, 4, nullptr);
}

bool Executor::HandleBrewResult(C_Alchemy* session, char code, wh::entitymodule::C_Item* item,
                                uint32_t count)
{
    if (session != m_pAlchemy || m_phase == E_Phase::Idle || !item)
        return false;   // not ours / manual brew / item-less verdict -> vanilla presentation

    CryStringT<char> name = "???", icon;
    float price = 0.0f;
    if (auto* classDef = item->m_pClassData)
        wh::guimodule::GetItemUiPresentation(classDef->m_guid, 0, name, icon, price);
    char line[256];
    if (count > 1)
        std::snprintf(line, sizeof line, "%s  x%u", name.c_str(), count);
    else
        std::snprintf(line, sizeof line, "%s", name.c_str());
    ShowCenterToast(CryStringT<char>(line));
    return true;
}

void Executor::BeginCooking()
{
    m_phase = E_Phase::Cooking;
    m_planCursor = 0;
    m_op = {};
    
    ShowPrompt("alchemy", "alch_autobrew_stop", true);
}

void Executor::ReturnToIdle()
{
    if (m_phase == E_Phase::Cooking)
        ShowPrompt("alchemy", "alch_autobrew_stop", false);
    m_phase = E_Phase::Idle;
    m_pendingRecipeId = 0;
    m_plan.clear();
    m_planCursor = 0;
    m_op = {};
}

// Every non-user-initiated teardown goes through here: a refusal with no trace reads as
// "nothing happened" in the field (the shot-ball report), so say why, on screen and in the log.
void Executor::AbortRun(const char* reason)
{
    if (auto* env = SSystemGlobalEnvironment::GetInstance(); env && env->pLog)
        env->pLog->LogAlways("[Autobrew] stopped: %s (recipe %u, op %zu/%zu)",
                             reason, m_pendingRecipeId, m_planCursor, m_plan.size());
    char line[128];
    std::snprintf(line, sizeof line, "Autobrew stopped: %s", reason);
    ShowCenterToast(CryStringT<char>(line));
    ReturnToIdle();
}

}  // namespace Autobrew
