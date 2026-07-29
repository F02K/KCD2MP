#include "KCSE/KCSEAPI.h"
#include "crysystem/SSystemGlobalEnvironment.h"
#include "Offsets/vtables/IInput.h"
#include "Offsets/vtables/IInputEventListener.h"
#include "Offsets/vtables/IFlashUI.h"
#include "Offsets/vtables/IUIElement.h"
#include "Offsets/vtables/I3DEngine.h"
#include "CryEngine/CryCommon/SInputEvent.h"
#include "guimodule/SUITypes.h"
#include "game/S_GameContext.h"
#include "playermodule/C_PlayerModule.h"
#include "playermodule/C_FastTravel.h"
#include "guimodule/C_UIMap.h"
#include "guimodule/C_UIApse.h"
#include "framework/E_UIApseView.h"

using wh::playermodule::C_FastTravel;
using wh::guimodule::C_UIMap;
using wh::framework::E_UIApseView;

#define FTT_LOG(fmt, ...) SSystemGlobalEnvironment::GetInstance()->pLog->LogAlways("[FastTravelTweaks] " fmt, ##__VA_ARGS__)

static constexpr float kNearMarkSq = 10.0f * 10.0f;   // world^2 (KCD2's cursor getter is world-space)

static C_FastTravel* GetFastTravel()
{
    auto* ctx = wh::game::S_GameContext::GetInstance();
    if (!ctx || !ctx->m_pPlayerModule)
        return nullptr;
    return ctx->m_pPlayerModule->m_pFastTravel;
}

// Don't cancel travel while the FT ambush prompt demands an answer.
static bool RandomEventPromptOpen()
{
    auto* map = C_UIMap::GetInstance();
    return map && map->m_randomEventDialog.m_isOpen;
}

// Deferred: the input broadcast is mid-iteration when OnInputEvent fires.
static void TryCancelFastTravel()
{
    auto* ft = GetFastTravel();
    if (ft && ft->IsFastTraveling() && !RandomEventPromptOpen())
    {
        FTT_LOG("CancelFastTravel()");
        ft->CancelFastTravel();
    }
}

// KCD1's MapMarker: cursor near the checkpoint mark on the open map.  KCD2's cursor getter
// already returns world (same space as the mark), so compare in world -- there is no
// coordFromGameWorldToMap.  Fills dest = {mark xy, terrain z}.
static bool MarkUnderCursor(C_UIMap*& map, Vec3& dest)
{
    map = C_UIMap::GetInstance();
    if (!map || map->m_modalDialog.m_isOpen)
        return false;
    auto* el = map->m_content.m_pElement;
    if (!el || !el->IsVisible())
        return false;

    Vec2 mark;
    map->GetCheckpointMarkPos(mark);
    if (mark.x == 0.0f && mark.y == 0.0f)
        return false;

    Vec2 cur;
    if (!map->GetMapCursorWorldPos(cur))
        return false;
    const float dx = cur.x - mark.x, dy = cur.y - mark.y;
    if (dx * dx + dy * dy > kNearMarkSq)
        return false;

    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env->p3DEngine)
        return false;
    dest = { mark.x, mark.y, env->p3DEngine->GetTerrainElevation(mark.x, mark.y) };
    return true;
}

// KCD1's `armed` test, straight from game state: SetDestination stored this dest into
// C_FastTravel::m_destination (verbatim) and the route still exists.
static bool RouteArmedTo(C_FastTravel* ft, C_UIMap* map, const Vec3& dest)
{
    return ft->m_destination.x == dest.x && ft->m_destination.y == dest.y && !map->m_ftPathNodes.empty();
}

// Hide the drawn preview line: "HideFastTravelPath"(false) on the ApseMap element.
static void HideFastTravelLine(C_UIMap* map)
{
    map->m_pFTDestination = nullptr;
    if (auto* el = map->m_content.m_pElement)
    {
        SUIArguments args;
        args.AddArgument(false);
        bool ok = el->CallFunction("HideFastTravelPath", args, nullptr, nullptr);
        FTT_LOG("HideFastTravelPath -> %d", (int)ok);
    }
}

class FastTravelTweaks : public Offsets::IInputEventListener {
    bool OnInputEvent(const Offsets::SInputEvent& e) override    // [0]
    {
        if (!(e.state & Offsets::eIS_Pressed))
            return false;

        if (e.keyId == Offsets::eKI_Escape || e.keyId == Offsets::eKI_XI_B)
        {
            auto* ft = GetFastTravel();
            if (ft && ft->IsFastTraveling() && !RandomEventPromptOpen())
            {
                FTT_LOG("cancel requested (key %d)", (int)e.keyId);
                KCSE::GetTaskInterface()->AddTask(TryCancelFastTravel);
                return true;
            }
        }
        else if (e.keyId == Offsets::eKI_E || e.keyId == Offsets::eKI_XI_A)
        {
            C_UIMap* map = nullptr;
            Vec3     dest;
            auto*    ft = GetFastTravel();
            if (!ft || ft->IsFastTraveling() || !MarkUnderCursor(map, dest))
                return false;

            if (!RouteArmedTo(ft, map, dest))        // 1st press: arm + preview the route
            {
                map->SetDestination(dest, false, false);
                map->m_ftPathPending = false;        // we draw it now; kill the deferred redraw
                FTT_LOG("arm: dest=(%.1f %.1f %.1f) nodes=%zu", dest.x, dest.y, dest.z, map->m_ftPathNodes.size());
                map->DrawFastTravelRoute(map->m_ftPathNodes);
                return true;
            }

            FTT_LOG("confirm: opening dialog");
            map->RemoveCheckpointMark();             // 2nd press: confirm + travel
            map->m_modalDialog.Open(
                CryStringT<char>("@ui_dlg_fasttravel_confirm"),
                nullptr,
                [dest] {
                    auto* f = GetFastTravel();
                    auto* m = C_UIMap::GetInstance();
                    if (!f || !m)
                        return;
                    FTT_LOG("confirm: yes -> StartTravel");
                    m->SetDestination(dest, false, false);
                    f->StartTravel();
                    m->m_ftPathPending = false;
                    HideFastTravelLine(m);
                    if (auto* apse = m->m_pApse)
                        apse->SetApseView(E_UIApseView::Map, 1, 0);
                },
                [] {
                    FTT_LOG("confirm: no");
                    if (auto* m = C_UIMap::GetInstance()) HideFastTravelLine(m);
                },
                CryStringT<char>("ui_Yes"),
                CryStringT<char>("ui_No"));
            return true;
        }
        return false;
    }
    bool OnInputEventUI(const void*) override { return false; }  // [1]
    int  GetPriority() const override { return 0; }             // [2]
    bool _vf3(const void*) override { return false; }           // [3]
} g_Listener;

KCSE_PLUGIN_INFO("Fast Travel Tweaks", "JerryYOJ", 1);
KCSE_PLUGIN_LOAD(kcse)
{
    kcse->GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) {
        if (msg->type == KCSE::IMessagingInterface::kMessage_DataLoaded) {
            SSystemGlobalEnvironment::GetInstance()->pInput->AddEventListener(&g_Listener);
            SSystemGlobalEnvironment::GetInstance()->pLog->LogAlways("[FastTravelTweaks] InputListener Registered");
        }
    });
    return true;
}
