// MCM — Mod Configuration Menu (KCSE plugin).
//
// Injects a "Mod Configuration" button into the ESC pause menu (RootIngame page)
// and drives the MCM flash element (Libs/UI/UIElements/MCM.xml + mcm.gfx, shipped
// in Mods/MCM/Data/MCM.pak).
//
// Architecture:
//  - Menu injection: MinHook on the RootIngame builder entry
//    — the thunk runs the original
//    builder, appends our button through the RAW flash tier ("AddBasicButton" with
//    a string id — no menu_buttons DB row, so the game's own click dispatch maps
//    it to id 0 and no-ops), re-issues "ShowPage", and listens on the Menu element
//    for the OnButton("mcm_open") click.
//  - Push: EVERY mod's settings go to flash once per open as granular typed calls
//    (BeginMods/AddMod/Add*/ShowMods). Flash caches them and owns mod switching
//    and per-setting resets; outbound traffic is onValueChanged(modIdx, itemIdx,
//    value) — indices mirror the push order — and the no-arg onBack. (The old
//    "EI transport kills args" era was a plugin-side TUIData tag-width bug; see
//    analysis/MCM/ei_transport_investigation.md.) A failed call re-runs the
//    whole push next frame.
//  - Keybinds are NATIVE (McmKeyListener): the engine only delivers Esc to the
//    movie as a GFx key event, so X/Esc are caught via IInput and forwarded
//    with CallFunction("SetInput", action); flash owns the search-focus
//    semantics and answers Esc with onBack when the search box isn't focused.
//  - No persistence: values live in memory for the session (reopening the menu
//    shows what the user set; a game restart shows config defaults). Every change
//    is broadcast to all KCSE plugins as an MCM::ValueChanged (api/MCM_API.h)
//    and fanned out to Lua callbacks registered via the MCM scriptbind
//    (src/scriptbind.h: MCM.RegisterValueChangeListener(modId, cb)) —
//    consuming mods own their own persistence.
//  - Settings sources (merged by id on every open, RebuildSettings):
//    Libs/UI/MCM/*.xml via the engine pak FS (parsed with pugixml), the
//    MCM::kMessage_BuildSettings broadcast for C++ plugins (ISettingsBuilder,
//    api/MCM_API.h), and Lua MCM.RegisterBuildSettingsListener callbacks
//    driving MCM.Add* (scriptbind.h).

#include <MinHook.h>

#include "KCSE/KCSEAPI.h"
#include "MCM_API.h"
#include "mcm.h"
#include "configmanager.h"
#include "scriptbind.h"
#include "settingsbuilder.h"
#include "listener/McmElementListener.h"
#include "listener/McmKeyListener.h"
#include "listener/MenuElementListener.h"
#include "REL.h"
#include "crysystem/SSystemGlobalEnvironment.h"
#include "Offsets/vtables/IFlashUI.h"
#include "Offsets/vtables/IUIElement.h"
#include "guimodule/SUITypes.h"
#include "guimodule/C_UIMenu.h"

#include <string>

using Offsets::IUIElement;
using wh::guimodule::C_UIMenu;

// --------------------------------------------------- flash call plumbing ----

// The element resolves CallFunction by the XML <function name=...>; fall back to
// the raw AS2 funcname in case the lookup keys on the other field.
static bool Call(IUIElement* el, const char* name, const char* funcname, const SUIArguments& args)
{
    if (el->CallFunction(name, args, nullptr, nullptr))
        return true;
    bool ok = funcname && el->CallFunction(funcname, args, nullptr, nullptr);
    if (!ok)
        MCM_LOG("WARN: CallFunction('%s'/'%s') failed", name, funcname ? funcname : "-");
    return ok;
}

// ----------------------------------------------------------- MCM element ----

bool g_mcmOpen = false;

// See mcm.h. Same session-cached pattern as GetMcmElement: the _smart_ptr
// holds a ref, so the pointer can't go stale under us.
IUIElement* GetMenuElement()
{
    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env || !env->pFlashUI)
        return nullptr;
    static _smart_ptr<IUIElement> el;
    if (!el)
        env->pFlashUI->GetUIElement(el, "Menu");
    return el.get();
}

// The pause menu is not key-driven: the game FORWARDS actions into Menu.gfx
// via CallFunction("SetInput", actionId, mode), so flash-level event
// exclusivity on our element can't stop menu navigation while MCM is open.
// Menu.gfx has its own kill switch for that feed — fc_setInput starts with
static void SetMenuBusy(bool busy)
{
    IUIElement* el = GetMenuElement();
    if (!el)
        return;
    SUIArguments a;
    a.AddArgument(busy);
    Call(el, "SetBusyProtection", "fc_setBusyProtection", a);
}

IUIElement* GetMcmElement()
{
    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env || !env->pFlashUI)
        return nullptr;

    static _smart_ptr<IUIElement> el;   // holds a ref for the session
    if (el)
        return el.get();
    env->pFlashUI->GetUIElement(el, "MCM");

    if (!el)
        MCM_LOG("ERROR: MCM element unavailable - is Mods/MCM/Data/MCM.pak installed?");

    return el.get();
}

// Each call's return is checked; any failure re-runs the whole push on a later
// frame (≤3 tries).
static bool PushAllOnce(IUIElement* el)
{
    bool ok = Call(el, "BeginMods", "fc_beginMods", SUIArguments());
    for (const Mod& m : g_mods) {
        SUIArguments a;
        a.AddArgument(m.id.c_str());
        a.AddArgument(m.name.c_str());
        ok &= Call(el, "AddMod", "fc_addMod", a);
        for (const Item& it : m.items) {
            SUIArguments b;
            b.AddArgument(m.id.c_str());
            switch (it.kind) {
                case Item::Cat:
                    b.AddArgument(it.label.c_str());
                    b.AddArgument(it.tip.c_str());
                    ok &= Call(el, "AddCategory", "fc_addCategory", b);
                    break;
                case Item::Toggle:
                    b.AddArgument(it.id.c_str());
                    b.AddArgument(it.label.c_str());
                    b.AddArgument(it.tip.c_str());
                    b.AddArgument(it.val != 0);
                    b.AddArgument(it.def != 0);
                    ok &= Call(el, "AddToggle", "fc_addToggle", b);
                    break;
                case Item::Dropdown:
                    b.AddArgument(it.id.c_str());
                    b.AddArgument(it.label.c_str());
                    b.AddArgument(it.tip.c_str());
                    b.AddArgument(it.options.c_str());
                    b.AddArgument((int)it.val);
                    b.AddArgument((int)it.def);
                    ok &= Call(el, "AddDropdown", "fc_addDropdown", b);
                    break;
                case Item::Slider:
                    b.AddArgument(it.id.c_str());
                    b.AddArgument(it.label.c_str());
                    b.AddArgument(it.tip.c_str());
                    b.AddArgument((float)it.mn);
                    b.AddArgument((float)it.mx);
                    b.AddArgument((float)it.step);
                    b.AddArgument((float)it.val);
                    b.AddArgument((float)it.def);
                    b.AddArgument(it.suffix.c_str());
                    ok &= Call(el, "AddSlider", "fc_addSlider", b);
                    break;
            }
        }
    }
    ok &= Call(el, "ShowMods", "fc_showMods", SUIArguments());
    return ok;
}

// First attempt runs inline (from OpenMCM); only the retry defers to the next
// frame via the task queue.
static void PushAllTask(int attempt)
{
    IUIElement* el = GetMcmElement();
    if (!el)
        return;
    if (PushAllOnce(el)) {
        if (attempt > 0)
            MCM_LOG("settings push recovered on attempt %d/3", attempt + 1);
        return;
    }
    if (attempt < 3) {
        MCM_LOG("WARN: settings push incomplete - retrying (%d/3)", attempt + 1);
        KCSE::GetTaskInterface()->AddTask([attempt] { PushAllTask(attempt + 1); });
    } else {
        MCM_LOG("ERROR: settings push failed after 3 attempts - menu may be incomplete");
    }
}

void CloseMCM()
{
    if (!g_mcmOpen)
        return;
    g_mcmOpen = false;
    SetMenuBusy(false);
    if (IUIElement* el = GetMcmElement())
        el->SetVisible(false);
    MCM_LOG("closed");
}

// Rebuild the settings model from all three sources; session values carry
// over by (modId, settingId).
static void RebuildSettings()
{
    ValueSnapshot values = SnapshotValues();
    g_mods.clear();
    ScanConfigXml();                                     // 1. pak XML configs
    if (KCSE::GetMessagingInterface()) {                 // 2. C++ plugins
        SettingsBuilder builder;
        KCSE::GetMessagingInterface()->Dispatch(
            MCM::kMessage_BuildSettings,
            static_cast<MCM::ISettingsBuilder*>(&builder), 0, nullptr);
    }
    NotifyLuaBuildSettings();                            // 3. lua build listeners
    ApplyValues(values);
    std::sort(g_mods.begin(), g_mods.end(),
              [](const Mod& a, const Mod& b) { return a.name < b.name; });
    MCM_LOG("settings rebuilt: %zu mod(s)", g_mods.size());
}

void OpenMCM()
{
    MCM_LOG("opening...");
    IUIElement* el = GetMcmElement();
    if (!el)
        return;                          // GetMcmElement already logged why
    RebuildSettings();

    el->AddEventListener(&g_mcmListener, "MCMPlugin");
    if (auto* env = SSystemGlobalEnvironment::GetInstance(); env && env->pInput)
        env->pInput->AddEventListener(&g_mcmKeyListener);

    el->SetVisible(true);
    PushAllTask(0);
    SetMenuBusy(true);
    g_mcmOpen = true;
    MCM_LOG("opened (%zu mods)", g_mods.size());
}

// -------------------------------------------------- value-change handling ---

// Broadcast to other KCSE plugins: sender "MCM", data = MCM::ValueChanged*
// (shared contract in api/MCM_API.h).
static void BroadcastChange(const Mod& mod, const Item& it)
{
    if (!KCSE::GetMessagingInterface())
        return;
    MCM::ValueChanged v{ mod.id.c_str(), it.id.c_str(), it.val };
    KCSE::GetMessagingInterface()->Dispatch(
        MCM::kMessage_ValueChanged, &v, sizeof(v), nullptr);
}

// Flash identifies the setting by INDICES (mod = push order, item = position
// in the mod's items vector): string args die intermittently in the EI
// transport, numbers always survive. String ids re-enter the world here, from
// our own model, for the broadcast.
void HandleValueChanged(int modIdx, int itemIdx, double v)
{
    if (modIdx < 0 || modIdx >= (int)g_mods.size()) {
        MCM_LOG("WARN: value for unknown mod index %d", modIdx);
        return;
    }
    Mod& mod = g_mods[modIdx];
    if (itemIdx < 0 || itemIdx >= (int)mod.items.size() ||
        mod.items[itemIdx].kind == Item::Cat) {
        MCM_LOG("WARN: value for unknown setting index %s.%d", mod.id.c_str(), itemIdx);
        return;
    }
    Item& it = mod.items[itemIdx];
    it.val = v;
    BroadcastChange(mod, it);
    NotifyLua(mod.id.c_str(), it.id.c_str(), it.val);
    MCM_LOG("%s.%s = %g", mod.id.c_str(), it.id.c_str(), v);
}

// ------------------------------------------------------------ menu button ---

static void InjectMenuButton(C_UIMenu* menu)
{
    IUIElement* el = menu->m_pElement;
    if (!el) {
        MCM_LOG("WARN: RootIngame page has no element - button not injected");
        return;
    }

    el->AddEventListener(&g_menuListener, "MCMPlugin");

    SUIArguments a;
    a.AddArgument("mcm_open");                 // raw string id (no menu_buttons row)
    a.AddArgument(0);                          // containerIndex
    a.AddArgument("Mod Configuration");
    a.AddArgument("Configure installed mods");
    a.AddArgument(false);                      // disable
    if (!el->CallFunction("AddBasicButton", a, nullptr, nullptr))
        MCM_LOG("WARN: AddBasicButton failed - pause menu button missing this open");
    menu->ShowPage();
    MCM_LOG("menu button injected");
}

// ------------------------------------------------------------------ hooks ---

namespace {

    class {
    public:
        static bool Install() {
            void* target = reinterpret_cast<void*>(REL::ID(13).address());
            if (MH_CreateHook(target, reinterpret_cast<void*>(&thunk),
                              reinterpret_cast<void**>(&orig)) != MH_OK)
                return false;

            return MH_EnableHook(target) == MH_OK;
        }

    protected:
        static void thunk(C_UIMenu* menu) {
            orig(menu);
            InjectMenuButton(menu);
        }

        static inline REL::Relocation<decltype(&thunk)> orig;
    } hkBuildRootIngame;

}  // namespace

bool InstallHooks()
{
    if (MH_Initialize() != MH_OK)
        return false;

    return hkBuildRootIngame.Install();
}

// -------------------------------------------------------------- KCSE glue ---

KCSE_PLUGIN_INFO("MCM", "JerryYOJ", 1);

KCSE_PLUGIN_LOAD(kcse)
{
    if (!InstallHooks())
        return false;

        
    kcse->GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) {
        if (msg->type == KCSE::IMessagingInterface::kMessage_PreDataLoaded)
            InitScriptBind();
    });
    return true;
}
