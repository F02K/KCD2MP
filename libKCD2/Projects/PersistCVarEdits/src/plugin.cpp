// PersistCVarEdits -- records CVar edits and replays them on the next launch
// through this mod's own mod.cfg (the game evals Mods/<mod>/mod.cfg at
// startup; values for not-yet-registered CVars are held by the console and
// applied when the CVar registers, so late-registered vars like the
// kcse_rpgparam_* family persist too).
//
// Mechanism: IConsoleVarSink. CXConsoleVariable*::Set* invoke
// CXConsole::OnBefore/OnAfterVarChange (IConsole slots [74]/[75]) on EVERY
// effective value change -- console lines, config eval, Lua System.SetCVar,
// C++ ICVar::Set -- and early-out on unchanged sets, so OnAfterVarChange is
// exactly "an edit happened" (RE notes in Offsets/vtables/IConsole.h).
// The sink registers at kMessage_DataLoaded, after all startup config churn;
// only changes made during the session are recorded.
//
// kcse_pce_persist (int): 1 = record edits (default), 0 = pause recording.
// The switch itself is always persisted so "off" survives restarts.

#include "KCSE/KCSEAPI.h"
#include "crysystem/SSystemGlobalEnvironment.h"
#include "crysystem/CXConsole.h"           // IConsoleVarSink + EVarFlags
#include "Offsets/vtables/ICVar.h"

#include <windows.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
#include <string>

#define PCE_LOG(fmt, ...) do { \
    if (auto* _env = SSystemGlobalEnvironment::GetInstance(); _env && _env->pLog) \
        _env->pLog->LogAlways("[PersistCVarEdits] " fmt, ##__VA_ARGS__); \
} while (0)

static constexpr const char* kPersistCVar = "kcse_pce_persist";

static int                                g_persist = 1;
static std::filesystem::path              g_cfgPath;
static std::map<std::string, std::string> g_entries;   // cvar -> value (sorted -> stable file)
static std::mutex                         g_lock;

static std::filesystem::path ModCfgPath()
{
    // <game>/Mods/<mod>/KCSE/Plugins/PersistCVarEdits.dll -> <game>/Mods/<mod>/mod.cfg
    HMODULE hm = nullptr;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                       GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       reinterpret_cast<LPCSTR>(&ModCfgPath), &hm);
    char buf[MAX_PATH]{};
    GetModuleFileNameA(hm, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path().parent_path().parent_path() / "mod.cfg";
}

static std::string Trim(const std::string& s)
{
    const size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos)
        return {};
    return s.substr(b, s.find_last_not_of(" \t\r\n") - b + 1);
}

// Seed g_entries from last session's file so a rewrite never forgets entries
// that weren't re-edited this session.
static void LoadExisting()
{
    std::ifstream f(g_cfgPath);
    if (!f)
        return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.rfind("--", 0) == 0)
            continue;
        const size_t eq = line.find('=');
        if (eq == std::string::npos)
            continue;
        const std::string name = Trim(line.substr(0, eq));
        if (!name.empty())
            g_entries[name] = Trim(line.substr(eq + 1));
    }
    PCE_LOG("loaded %zu persisted edit(s) from %s", g_entries.size(), g_cfgPath.string().c_str());
}

static void WriteFileLocked()
{
    std::ofstream f(g_cfgPath, std::ios::trunc);
    if (!f) {
        PCE_LOG("ERROR: cannot write %s", g_cfgPath.string().c_str());
        return;
    }
    f << "-- PersistCVarEdits - auto-generated; the game applies this file at startup.\n"
         "-- Delete a line to forget that edit. kcse_pce_persist = 0 pauses recording.\n";
    for (const auto& [name, value] : g_entries)
        f << name << " = " << value << "\n";
}

static void RecordEdit(const char* name, const char* value)
{
    if (!value || std::strpbrk(value, "\r\n"))
        return;
    std::lock_guard lock(g_lock);
    auto it = g_entries.find(name);
    if (it != g_entries.end() && it->second == value)
        return;                              // replay of an already-persisted value
    g_entries[name] = value;
    WriteFileLocked();
    PCE_LOG("%s = %s", name, value);
}

namespace {
class CVarEditSink : public IConsoleVarSink {
    bool OnBeforeVarChange(ICVar*, const char*) override { return true; }
    void OnAfterVarChange(ICVar* pVar) override
    {
        const char* name = pVar->GetName();
        if (!name || !*name)
            return;
        if (_stricmp(name, kPersistCVar) == 0) {     // the switch itself always persists
            RecordEdit(kPersistCVar, pVar->GetString());
            return;
        }
        if (!g_persist)
            return;
        if (pVar->GetFlags() & (VF_READONLY | VF_CONST_CVAR))
            return;                                  // engine-internal, not a user edit
        RecordEdit(name, pVar->GetString());
    }
};
CVarEditSink g_sink;
}

KCSE_PLUGIN_INFO("Persist CVar Edits", "JerryYOJ", 1);
KCSE_PLUGIN_LOAD(kcse)
{
    g_cfgPath = ModCfgPath();
    LoadExisting();

    kcse->GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) {
        auto* env = SSystemGlobalEnvironment::GetInstance();
        if (!env || !env->pConsole)
            return;
        if (msg->type == KCSE::IMessagingInterface::kMessage_PreDataLoaded) {
            // registered before mod scripts run; a persisted kcse_pce_persist
            // line was held by the console since mod.cfg eval and applies here
            env->pConsole->RegisterCVarInt(kPersistCVar, &g_persist, 1, VF_NULL,
                                           "PersistCVarEdits: 1 = record cvar edits into mod.cfg, 0 = pause");
        } else if (msg->type == KCSE::IMessagingInterface::kMessage_DataLoaded) {
            env->pConsole->AddConsoleVarSink(&g_sink);
            PCE_LOG("cvar sink armed (persist=%d, %zu entries)", g_persist, g_entries.size());
        }
    });

    return true;
}
