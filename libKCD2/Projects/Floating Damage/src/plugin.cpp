#include <MinHook.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "KCSE/KCSEAPI.h"
#include "crysystem/CCryAction.h"
#include "REL.h"

#include "mcm.h"

namespace {

#define FD_LOG(fmt, ...) do { \
    if (auto* _env = SSystemGlobalEnvironment::GetInstance(); _env && _env->pLog) \
        _env->pLog->LogAlways("[FloatingDamage] " fmt, ##__VA_ARGS__); \
} while (0)


std::shared_ptr<Offsets::IFlashPlayer> GetHudFlashPlayer() {
    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env || !env->pFlashUI) return nullptr;

    _smart_ptr<Offsets::IUIElement> elem;
    env->pFlashUI->GetUIElement(elem, "hud");
    if (!elem) return nullptr;

    return elem->GetFlashPlayer();
}

// Resolves damageOverlay and invokes floatingdmg.as's ShowDamage(amount, isHealthDmg,
// xPx, yPx, sizePx, isFatal). Shared by the console test command and the live damage
// hook below. Pixel coordinates, not fractions -- see floatingdmg.as's header comment
// for why.
bool InvokeShowDamage(float amount, bool isHealthDmg, float xPx, float yPx, float sizePx, bool isFatal) {
    auto fp = GetHudFlashPlayer();
    if (!fp) return false;

    Offsets::FlashVarPtr mc;
    if (!fp->GetVariable("_root.damageOverlay", mc.put()) || !mc) return false;

    SFlashVarValue callArgs[6] = { amount, isHealthDmg, xPx, yPx, sizePx, isFatal };
    return mc->Invoke("ShowDamage", callArgs, 6, nullptr);
}

// CFlashUIElement::Init -- the single point every element-movie load funnels
// through (game-initiated or lazy via GetFlashPlayer); same injection pattern
// as CompassOverhaul (RE/Projects/CompassOverhaul/src/plugin.cpp).
class{
public:
    static bool Install() {
        void* target = reinterpret_cast<void*>(REL::ID(24).address());
        if (MH_CreateHook(target, reinterpret_cast<void*>(&thunk),
            reinterpret_cast<void**>(&orig)) != MH_OK)
            return false;

        return MH_EnableHook(target) == MH_OK;
    }

    static void Uninstall() {
        void* target = reinterpret_cast<void*>(REL::ID(24).address());
        MH_RemoveHook(target);
    }

protected:
    static bool thunk(CFlashUIElement* self, bool load) {
        const bool ok = orig(self, load);
        if (!ok || !load)
            return ok;

        if (self->m_sName == "hud") {
            const auto& fp = self->m_pFlashPlayer;
            if (!fp || fp->IsAvailable("_root.damageOverlay"))
                return ok;

            Offsets::FlashVarPtr root, mc;
            if (!fp->GetVariable("_root", root.put()) || !root) return ok;
            if (!root->CreateEmptyMovieClip(mc.put(), "damageOverlay") || !mc) return ok;

            SFlashVarValue url("floatingdmg.gfx");   // resolved relative to the parent movie dir "Libs/UI/"
            if (!mc->Invoke("loadMovie", &url, 1, nullptr)) return ok;

            // The stopped HUD timeline only drains its load queue on a frame-step,
            // so service our entry now (see GFxMovieRoot.h).
            if (auto* movie = static_cast<CFlashPlayer*>(fp.get())->GetMovieRoot())
                movie->ProcessLoadQueue();

            FD_LOG("damageOverlay injected");
        }

        return ok;
    }

    static inline REL::Relocation<decltype(&thunk)> orig;
}hkCFlashUIElementInit;


// Linearly maps |amount| from [g_minScaleDamage, g_maxScaleDamage] to
// [g_minTextSizePx, g_maxTextSizePx] (both MCM-configurable, see mcm.h), clamped at both ends.
float ComputeTextSize(float amount) {
    const float range = g_maxScaleDamage - g_minScaleDamage;
    if (range <= 0.0f) return g_minTextSizePx;
    const float t = std::clamp((std::fabs(amount) - g_minScaleDamage) / range, 0.0f, 1.0f);
    return g_minTextSizePx + t * (g_maxTextSizePx - g_minTextSizePx);
}

// Inverse-linear distance decay for popup size: full scale at/inside g_decayStartDist
// meters from the player, then shrinks like real perspective (half at 2x the distance,
// a third at 3x), floored at g_minDistanceScale. Both knobs MCM-configurable (mcm.h);
// slider at 0 m disables decay entirely. Player = IGameFramework::GetClientEntity
// (slot [66], body-verified -- see Offsets/vtables/IGameFramework.h); on lookup
// failure the popup just keeps full size.
float ComputeDistanceScale(const Vec3& worldPos) {
    if (g_decayStartDist <= 0.0f) return 1.0f;

    auto* fw = CCryAction::GetInstance();
    auto* playerEntity = fw ? fw->GetClientEntity() : nullptr;
    if (!playerEntity) return 1.0f;

    Vec3 eye{};
    playerEntity->GetWorldPos(eye);
    const float dx = worldPos.x - eye.x, dy = worldPos.y - eye.y, dz = worldPos.z - eye.z;
    const float dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (dist <= g_decayStartDist) return 1.0f;
    return std::max(g_decayStartDist / dist, g_minDistanceScale);
}

// yOffsetFrac is a fraction of screen HEIGHT, not the xPx/yPx crossing into Flash below --
// this stays native-side only (multiplied by h, our own renderer-verified height) before
// InvokeShowDamage ever sees it, so it never depends on GFx's Stage dimensions the way the
// old xFrac/yFrac-to-Flash convention did. Resolution-independent on purpose: a fixed pixel
// nudge would look proportionally larger on a smaller viewport.
void ShowDamageAtWorldPos(float amount, bool isHealthDmg, const Vec3& worldPos, float yOffsetFrac, bool isFatal, float sizeScale) {
    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env || !env->pRenderer) return;

    float sx, sy, sz;
    env->pRenderer->ProjectToScreen(worldPos.x, worldPos.y, worldPos.z, &sx, &sy, &sz);

    const float w = static_cast<float>(env->pRenderer->GetWidth());
    const float h = static_cast<float>(env->pRenderer->GetHeight());
    const float pxX = sx * 0.01f * w;
    const float pxY = sy * 0.01f * h;
    const bool offscreen = pxX < 0.0f || pxY < 0.0f || std::fabs(pxX) > w || std::fabs(pxY) > h || sz > 1.0f;

    if (offscreen) return;

    // sizeScale (distance decay) deliberately multiplies AFTER the damage->px mapping,
    // so a far popup can drop below the MCM "Min Text Size" -- that floor is the
    // damage-scale floor; the decay floor is g_minDistanceScale.
    InvokeShowDamage(amount, isHealthDmg, pxX, pxY + yOffsetFrac * h, ComputeTextSize(amount) * sizeScale, isFatal);
}

// Collision/BlockDamage/ScriptedDamage/VelocityImpact producers never write m_hitPosition --
// it's ctor-zero for those types, not a real contact point. MissileHit is structurally
// reliable (EventPhysCollision::pt). MeleeHit is only a CANDIDATE: three synthetic producers
// build their hit-details via sub_180D508C0, which never initializes its +0x00..+0x23 Vec3s,
// and that uninitialized stack memory propagates unconditionally into m_hitPosition
// (sub_180D568B0 -> sub_180727B0C). No validity flag exists anywhere in the chain, so melee
// needs a runtime sanity check (finite + non-zero + near the victim).
bool IsHitPositionValid(const wh::rpgmodule::S_DamageEventData* ctx, const Vec3& root) {
    using wh::rpgmodule::E_DamageEventType;
    if (ctx->m_type != E_DamageEventType::MeleeHit && ctx->m_type != E_DamageEventType::MissileHit)
        return false;

    const Vec3& pos = ctx->m_hitPosition;
    if (!std::isfinite(pos.x) || !std::isfinite(pos.y) || !std::isfinite(pos.z))
        return false;
    if (std::fabs(pos.x) + std::fabs(pos.y) + std::fabs(pos.z) <= 0.01f)
        return false;

    if (ctx->m_type == E_DamageEventType::MeleeHit) {
        const float dx = pos.x - root.x, dy = pos.y - root.y, dz = pos.z - root.z;
        constexpr float kMaxPlausibleMeters = 4.0f;
        if (dx * dx + dy * dy + dz * dz > kMaxPlausibleMeters * kMaxPlausibleMeters)
            return false;
    }
    return true;
}

Vec3 GetPopupAnchor(const wh::rpgmodule::S_DamageEventData* ctx) {
    auto* victimEntity = ctx->m_pVictim ? ctx->m_pVictim->GetBoundEntity() : nullptr;

    Vec3 root{};
    if (victimEntity)
        victimEntity->GetWorldPos(root);
    else return ctx->m_hitPosition;

    if (!IsHitPositionValid(ctx, root)) {
        AABB bbox;
        victimEntity->GetWorldBounds(bbox);
        const Vec3 bodyCenter = bbox.IsReset() ? root : (bbox.min + bbox.max) * 0.5f;
        FD_LOG("Fallen back to body-center pos for %s", ctx->m_pVictim->m_name.c_str());
        return bodyCenter;
    }

    Vec3 pos = ctx->m_hitPosition;

    return pos;
}

// S_DamageEventData::Dispatch -- IDA-renamed from sub_18072704C
class{
public:
    static bool Install() {
        void* target = reinterpret_cast<void*>(REL::ID(20).address());
        if (MH_CreateHook(target, reinterpret_cast<void*>(&thunk),
            reinterpret_cast<void**>(&orig)) != MH_OK)
            return false;

        return MH_EnableHook(target) == MH_OK;
    }

    static void Uninstall() {
        void* target = reinterpret_cast<void*>(REL::ID(20).address());
        MH_RemoveHook(target);
    }

protected:
    static int64_t thunk(wh::rpgmodule::S_DamageEventData* ctx) {
        const int64_t ret = orig(ctx);

        if (ctx->m_healthDamage > 0.0f || ctx->m_staminaDamage > 0.0f) {
            // Attacker resolved for both the only-player filter and the decay exemption below.
            auto* attackerEntity = ctx->m_pAttacker ? ctx->m_pAttacker->GetBoundEntity() : nullptr;
            auto* attackerActor = attackerEntity
                ? wh::game::S_GameContext::GetInstance()->GetActorById(attackerEntity->GetId())
                : nullptr;
            const bool playerCaused = attackerActor && attackerActor->IsPlayer();

            if (g_onlyShowPlayerAttacks && !playerCaused)
                return ret;

            const Vec3 pos = GetPopupAnchor(ctx);
            // Player-dealt hits stay full size at any range (aimed bow shots must read
            // clearly); everything else shrinks with distance from the player.
            const float sizeScale = playerCaused ? 1.0f : ComputeDistanceScale(pos);

            if (ctx->m_healthDamage > 0.0f) {

                auto* entity = ctx->m_pVictimEntity;
                auto* actor = entity ? wh::game::S_GameContext::GetInstance()->GetActorById(entity->GetId()) : nullptr;
                auto* soul = ctx->m_pVictim;
                const float healthBefore = actor ? actor->GetHealth() : 0.0f;
                const bool isFatal = actor && soul &&
                    healthBefore > 0.0f &&
                    !ctx->m_nonlethalClampApplied &&
                    healthBefore - ctx->m_healthDamage <= 0.0f &&
                    soul->GetDerivedStat(wh::rpgmodule::E_DerivedStat::Imm) <= 0.0f;

                ShowDamageAtWorldPos(ctx->m_healthDamage, true, pos, 0.023, isFatal, sizeScale);
            }
            if (ctx->m_staminaDamage > 0.0f)
                ShowDamageAtWorldPos(ctx->m_staminaDamage, false, pos, 0.046, false, sizeScale);
        }

        return ret;
    }

    static inline REL::Relocation<decltype(&thunk)> orig;
}hkDamageDispatch;

// kcse_fd_test [amount] [isHealthDmg 0/1] [xFrac] [yFrac] [isFatal 0/1] -- calls
// damageOverlay's ShowDamage(amount, isHealthDmg, xPx, yPx, sizePx, isFatal)
// (floatingdmg.as) directly, same call pattern as FastTravelTweaks'
// HideFastTravelPath. xFrac/yFrac stay 0..1 fractions here for convenient console
// typing -- converted to real renderer pixels below before crossing into Flash,
// same as the live world-position path. All args optional. A negative amount with
// isHealthDmg=1 is healing (green) -- e.g. "kcse_fd_test -20 1".
void ConsoleTestDamage(IConsoleCmdArgs* args) {
    const float amount      = args->GetArgCount() > 1 ? static_cast<float>(std::atof(args->GetArg(1))) : 42.0f;
    const bool  isHealthDmg = args->GetArgCount() > 2 ? std::atoi(args->GetArg(2)) != 0 : false;
    const float xFrac       = args->GetArgCount() > 3 ? static_cast<float>(std::atof(args->GetArg(3))) : 0.5f;
    const float yFrac       = args->GetArgCount() > 4 ? static_cast<float>(std::atof(args->GetArg(4))) : 0.4f;
    const bool  isFatal     = args->GetArgCount() > 5 ? std::atoi(args->GetArg(5)) != 0 : false;

    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env || !env->pRenderer) {
        FD_LOG("kcse_fd_test: no renderer");
        return;
    }
    const float w = static_cast<float>(env->pRenderer->GetWidth());
    const float h = static_cast<float>(env->pRenderer->GetHeight());

    if (!InvokeShowDamage(amount, isHealthDmg, xFrac * w, yFrac * h, ComputeTextSize(amount), isFatal))
        FD_LOG("kcse_fd_test: ShowDamage invoke failed (HUD/overlay not ready?)");
}

}  // namespace

bool InstallHooks() {
    if (MH_Initialize() != MH_OK)
        return false;

    return hkCFlashUIElementInit.Install() && hkDamageDispatch.Install();
}

KCSE_PLUGIN_INFO("Floating Damage", "JerryYOJ", 1);
KCSE_PLUGIN_LOAD(kcse)
{
    if (!InstallHooks())
        return false;

    kcse->GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) {
        if (msg->type != KCSE::IMessagingInterface::kMessage_PreDataLoaded)
            return;
        auto* env = SSystemGlobalEnvironment::GetInstance();
        if (!env || !env->pConsole)
            return;
        env->pConsole->AddCommand("kcse_fd_test", &ConsoleTestDamage, VF_NULL,
            "Floating Damage: test-spawn a damage popup. Usage: kcse_fd_test [amount] [isHealthDmg 0/1] [xFrac] [yFrac] [isFatal 0/1]");
    });

    // Soft dependency: if MCM.dll isn't installed, nothing ever broadcasts sender="MCM"
    // messages, so this listener simply never fires and the g_* defaults stand.
    MCM::ListenForMessages(&HandleMcmMessage);

    return true;
}
