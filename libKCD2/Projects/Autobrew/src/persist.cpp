#include "persist.h"

#include <windows.h>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>

namespace Autobrew {

namespace {

std::filesystem::path              g_cfgPath;
std::map<std::string, std::string> g_entries;   // cvar -> value (sorted -> stable file)

std::filesystem::path ModCfgPath()
{
    // <game>/Mods/Autobrew/KCSE/Plugins/Autobrew.dll -> <game>/Mods/Autobrew/mod.cfg
    HMODULE hm = nullptr;
    GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                       GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                       reinterpret_cast<LPCSTR>(&ModCfgPath), &hm);
    char buf[MAX_PATH]{};
    GetModuleFileNameA(hm, buf, MAX_PATH);
    return std::filesystem::path(buf).parent_path().parent_path().parent_path() / "mod.cfg";
}

std::string Trim(const std::string& s)
{
    const size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos)
        return {};
    return s.substr(b, s.find_last_not_of(" \t\r\n") - b + 1);
}

void WriteCfg()
{
    std::ofstream f(g_cfgPath, std::ios::trunc);
    if (!f)
        return;
    f << "-- Autobrew settings - auto-generated from the MCM menu; the game applies this\n"
         "-- file at startup.  Delete a line to return that setting to its default.\n";
    for (const auto& [name, value] : g_entries)
        f << name << " = " << value << "\n";
}

}  // namespace

void LoadPersistedEdits()
{
    g_cfgPath = ModCfgPath();
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
}

void PersistSetting(const char* cvarName, double value, bool isFloat)
{
    char text[64];
    if (isFloat)
        std::snprintf(text, sizeof text, "%g", value);
    else
        std::snprintf(text, sizeof text, "%d", static_cast<int>(value));
    auto it = g_entries.find(cvarName);
    if (it != g_entries.end() && it->second == text)
        return;   // unchanged (e.g. slider drag passing through the stored value)
    g_entries[cvarName] = text;
    WriteCfg();
}

}  // namespace Autobrew
