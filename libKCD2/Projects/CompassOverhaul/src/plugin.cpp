#include <MinHook.h>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "KCSE/KCSEAPI.h"
#include "REL.h"
#include "guimodule/C_UICompass.h"
#include "guimodule/C_UIQuestLog.h"
#include "guimodule/C_GUIModule.h"
#include "guimodule/C_CompassMark.h"
#include "guimodule/E_MarkType.h"
#include "questmodule/C_Objective.h"
#include "questmodule/C_Quest.h"
#include "framework/C_LocalizedString.h"
#include "CryEngine/CryCommon/CFlashUIElement.h"
#include "CryEngine/CryCommon/CFlashPlayer.h"
#include "Offsets/vtables/IFlashVariableObject.h"

namespace gm = wh::guimodule;
namespace qm = wh::questmodule;

namespace {

    std::map<std::string, std::string> g_objectiveNames;   // "marker<id>" -> localized objective name

    void PushTable(Offsets::IFlashPlayer* fp, const char* path,
                   const std::map<std::string, std::string>& table) {
        std::vector<const char*> flat;
        flat.reserve(table.size() * 2);
        for (const auto& [k, v] : table) {
            flat.push_back(k.c_str());
            flat.push_back(v.c_str());
        }
        fp->SetVariableArray(Offsets::IFlashPlayer::FVAT_ConstStrPtr, path, 0,
            flat.data(), static_cast<unsigned int>(flat.size()));
    }

    void PushObjectiveNames(Offsets::IFlashPlayer* fp) {
        PushTable(fp, "_root.compassOverlay.co_names", g_objectiveNames);
    }

    void PushTypeNames(Offsets::IFlashPlayer* fp) {
        static const std::map<std::string, std::string> kKeySuffixOverride = {
            { "HorseTrader",     "horse_trader" },
            { "ArcheryArena",    "archery_arena" },
            { "FistFight",       "fist_fight" },
            { "HuntingSpot",     "hunting_spot" },
            { "HuntingSpotBoar", "hunting_spot_boar" },
            { "HuntingSpotRoe",  "hunting_spot_roe_deer" },
            { "HuntingSpotWolf", "hunting_spot_wolf" },
            { "ForestGarden",    "forest_garden" },
            { "CampEnemy",       "camp_enemy" },
            { "ConcCross",       "conc_cross" },
            { "GeneralPoi",      "general_poi" },
            { "VegetableShop",   "vegetable_shop" },
            { "FastTravelLevel", "fasttravel_level" },
            { "FastTravelSedlec","fasttravel_sedlec" },
            { "SmokingFood",     "smokehouse" },
            { "DryingFood",      "dryer" },
            { "Washing",         "laundry" },
            { "Indulgences",     "indulgenceBox" },
            { "FishingSpot",     "fisherman" },
            { "MineEnrtrance",   "undegroundEntrance" },   // (sic, matches the game's own key typo)
        };

        std::map<std::string, std::string> table;
        for (int t = 0; t <= gm::E_MarkType::FastTravelSedlec; ++t) {
            const char* name = gm::GetMarkTypeName(static_cast<gm::E_MarkType::Type>(t));
            if (!name || !*name || (name[0] == '?' && !name[1])) continue;

            auto it = kKeySuffixOverride.find(name);
            std::string key = "ui_maplegend_" + (it != kKeySuffixOverride.end() ? it->second : std::string(name));
            CryStringT<char> loc = wh::framework::C_LocalizedString::Resolve(key.c_str());
            if (loc.empty() || key == loc.c_str()) continue;   // no loc entry -> hide
            table[name] = loc.c_str();
        }
        PushTable(fp, "_root.compassOverlay.co_typeNames", table);
    }

    class{
    public:
        static bool Install() {
            void* target = reinterpret_cast<void*>(REL::ID(36).address());
            if (MH_CreateHook(target, reinterpret_cast<void*>(&thunk),
                reinterpret_cast<void**>(&orig)) != MH_OK)
                return false;

            return MH_EnableHook(target) == MH_OK;
        }

        static void Uninstall() {
            void* target = reinterpret_cast<void*>(REL::ID(36).address());
            MH_RemoveHook(target);
        }

    protected:
        static void thunk(gm::C_UIQuestLog* self, qm::C_Objective* obj, qm::C_Quest* quest) {
            auto* compass = self->m_pModule ? self->m_pModule->GetUICompass() : nullptr;
            const size_t before = compass ? compass->m_marks.size() : 0;

            orig(self, obj, quest);

            if (!compass || !obj) return;
            CryStringT<char> name = wh::framework::C_LocalizedString::Resolve(obj->m_locText.m_text);
            if (name.empty()) return;
            bool added = false;
            
            for (size_t i = before; i < compass->m_marks.size(); ++i) {
                const auto& mark = compass->m_marks[i];
                if (!mark) continue;
                g_objectiveNames["marker" + std::string(mark->m_id.c_str())] = name.c_str();
                added = true;
            }
            if (added) {
                auto* hud = static_cast<CFlashUIElement*>(compass->m_pElement);
                if (hud && hud->m_pFlashPlayer)
                    PushObjectiveNames(hud->m_pFlashPlayer.get());
            }
        }

        static inline REL::Relocation<decltype(&thunk)> orig;
    }hkAddObjectiveCompassMarkers;

    // CFlashUIElement::Init -- the single point every element-movie load funnels
    // through (game-initiated or lazy via GetFlashPlayer): loads Libs/UI/<file>,
    // creates the player, fires the OnInit listeners. Injecting post-orig follows
    // the game's own load timing instead of forcing it (the old C_UICompass::Init
    // site called GetFlashPlayer while the HUD was deliberately unloaded during
    // the new-game intro, force-loading the movie early -> black screen).
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
            
            if(self->m_sName == "hud"){
                const auto& fp = self->m_pFlashPlayer;
                if (!fp || fp->IsAvailable("_root.compassOverlay")) return ok;

                Offsets::FlashVarPtr root, mc;
                if (!fp->GetVariable("_root", root.put()) || !root) return ok;
                if (!root->CreateEmptyMovieClip(mc.put(), "compassOverlay") || !mc) return ok;

                SFlashVarValue url("compass.gfx");   // resolved relative to the parent movie dir "Libs/UI/"
                if (!mc->Invoke("loadMovie", &url, 1, nullptr)) return ok;

                // The stopped HUD timeline only drains its load queue on a frame-step,
                // so service our entry now (see GFxMovieRoot.h).
                if (auto* movie = static_cast<CFlashPlayer*>(fp.get())->GetMovieRoot())
                    movie->ProcessLoadQueue();

                PushObjectiveNames(fp.get());
                PushTypeNames(fp.get());
            }

            
            return ok;
        }

        static inline REL::Relocation<decltype(&thunk)> orig;
    }hkCFlashUIElementInit;

}  // namespace

bool InstallHooks() {
    if (MH_Initialize() != MH_OK)
        return false;

    return hkAddObjectiveCompassMarkers.Install()
        && hkCFlashUIElementInit.Install();
}

KCSE_PLUGIN_INFO("CompassOverhaul", "JerryYOJ", 1);
KCSE_PLUGIN_LOAD(kcse)
{
    return InstallHooks();
}
