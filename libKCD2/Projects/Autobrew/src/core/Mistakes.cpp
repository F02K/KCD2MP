#include "core/Mistakes.h"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include "crysystem/SSystemGlobalEnvironment.h"
#include "guimodule/ItemUiPresentation.h"
#include "playermodule/C_Alchemy.h"
#include "rpgmodule/C_Soul.h"
#include "rpgmodule/S_RpgParams.h"

#include "mcm.h"

namespace Autobrew {

namespace {

float Clamp01(float v) { return std::min(std::max(v, 0.0f), 1.0f); }

// Localized display name for a log entry; the item-presentation registry is keyed by exactly
// these class-def guids.  Falls back to the guid's first dword.
CryStringT<char> ItemName(const CryGUID& classGuid)
{
    CryStringT<char> name, icon;
    float price = 0.0f;
    wh::guimodule::GetItemUiPresentation(classGuid, 0, name, icon, price);
    if (name.empty()) {
        uint32_t head = 0;
        std::memcpy(&head, &classGuid, sizeof head);
        char buf[16];
        std::snprintf(buf, sizeof buf, "%08x", head);
        name = buf;
    }
    return name;
}

}  // namespace

void MistakeEngine::BeginBatch(const wh::rpgmodule::C_Soul* soul)
{
    m_log[0] = '\0';
    m_logLen = 0;
    m_active = false;
    m_hardcore = false;
    m_sigma = m_slipP = m_majorP = 0.0f;
    if (g_mistakeMode <= 0 || !soul)
        return;
    m_hardcore = g_mistakeMode >= 2;

    auto* defs = wh::rpgmodule::S_RpgParamDefs::Get();
    const float tolBase = defs->AlchemyToleranceBase.Value().asFloat;
    const float tol = wh::playermodule::C_Alchemy::GetBrewTolerance();
    const float L = soul->GetSkillFraction(kSkillIdAlchemy);
    const float apq = soul->GetPerkStatModifier(wh::rpgmodule::E_PerkStat::apq);

    const float intensity = std::min(std::max(g_mistakeIntensity, 0.0f), 4.0f);
    const float rate = m_hardcore ? kHardcoreRateMult : 1.0f;
    const float steadiness = Clamp01(L + apq);

    // sigma anchored to the LEVEL-0 tolerance: a one-sigma miss is exactly the error a fresh
    // brewer's band forgives -- pass/fail then falls out of the game's live band vs skill.
    m_sigma  = std::max(tolBase, 0.0f) * intensity * rate * (1.0f - Clamp01(apq));
    m_slipP  = Clamp01(kSlipBaseProbability * intensity * rate * (1.0f - steadiness));
    m_majorP = m_hardcore ? Clamp01(kMajorBaseProbability * intensity * (1.0f - steadiness))
                          : 0.0f;
    m_active = m_sigma > 0.0f || m_slipP > 0.0f || m_majorP > 0.0f;
    if (m_active)
        AppendLog("L=%.2f apq=%.2f tol=%.3f sigma=%.3f slipP=%.2f majorP=%.2f%s",
                  L, apq, tol, m_sigma, m_slipP, m_majorP, m_hardcore ? " HARDCORE" : "");
}

bool MistakeEngine::RollSloppyDried(const CryGUID& freshClassGuid)
{
    if (!m_active || !Roll(m_slipP))
        return false;
    AppendLog(" | dried %s", ItemName(freshClassGuid).c_str());
    return true;
}

void MistakeEngine::RollSlips(std::vector<S_IngredientPlan>& rows,
                              const std::function<uint32_t(const CryGUID&)>& countAvailable)
{
    if (!m_active || m_slipP <= 0.0f)
        return;
    for (auto& e : rows) {
        if (e.milled && Roll(m_slipP)) {
            e.milled = false;
            AppendLog(" | skip-grind %s", ItemName(e.guid).c_str());
        } else if (e.mustNotMill && !e.milled && Roll(m_slipP)) {
            e.milled = true;
            AppendLog(" | wrong-grind %s", ItemName(e.guid).c_str());
        }
        if (Roll(m_slipP)) {
            // One count slip per row; direction picked among what's actually valid: under
            // needs qty >= 2 (qty 0 would kill the recipe's Trivial gate), over needs the
            // player to hold one spare unit.
            const bool canUnder = e.qty >= 2;
            const bool canOver  = countAvailable(e.guid) >= e.qty + 1;
            if (canUnder && (!canOver || Roll(0.5f))) {
                --e.qty;
                AppendLog(" | under-count %s", ItemName(e.guid).c_str());
            } else if (canOver) {
                ++e.qty;
                AppendLog(" | over-count %s", ItemName(e.guid).c_str());
            }
        }
    }
}

float MistakeEngine::PerturbWindowSeconds(float trueSeconds)
{
    if (!m_active || m_sigma <= 0.0f || trueSeconds <= 0.0f)
        return trueSeconds;
    std::normal_distribution<float> dist(0.0f, m_sigma);
    float eps = dist(m_rng);
    const float cap = std::min(kEpsilonClampSigmas * m_sigma, 1.0f);
    eps = std::min(std::max(eps, -cap), cap);
    AppendLog(" | boil %+d%%", static_cast<int>(eps * 100.0f));
    return std::max(0.0f, trueSeconds * (1.0f + eps));
}

bool MistakeEngine::RollTempSwap()
{
    if (!m_active || !Roll(m_majorP))
        return false;
    AppendLog(" | temp-swap");
    return true;
}

bool MistakeEngine::RollSkipDistill()
{
    if (!m_active || !Roll(m_majorP))
        return false;
    AppendLog(" | skip-distill");
    return true;
}

bool MistakeEngine::RollSkipPowderGrind()
{
    if (!m_active || !Roll(m_majorP))
        return false;
    AppendLog(" | skip-powder-grind");
    return true;
}

void MistakeEngine::FlushLog(uint32_t recipeId)
{
    if (!m_active)
        return;
    if (auto* env = SSystemGlobalEnvironment::GetInstance(); env && env->pLog)
        env->pLog->LogAlways("[Autobrew] mistakes (recipe %u): %s", recipeId, m_log);
}

bool MistakeEngine::Roll(float p)
{
    if (p <= 0.0f)
        return false;
    return std::uniform_real_distribution<float>(0.0f, 1.0f)(m_rng) < p;
}

void MistakeEngine::AppendLog(const char* fmt, ...)
{
    if (m_logLen >= static_cast<int>(sizeof m_log) - 1)
        return;
    va_list args;
    va_start(args, fmt);
    const int n = std::vsnprintf(m_log + m_logLen, sizeof m_log - m_logLen, fmt, args);
    va_end(args);
    if (n > 0)
        m_logLen = std::min(m_logLen + n, static_cast<int>(sizeof m_log) - 1);
}

}  // namespace Autobrew
