// Settings model + config discovery. Three sources feed g_mods, merged by id
// on every rebuild (plugin.cpp RebuildSettings):
//   1. Libs/UI/MCM/*.xml via the engine pak FS (ScanConfigXml — mods ship the
//      file inside their own .pak; loose Data/Libs/UI/MCM works too),
//   2. C++ plugins via the MCM::kMessage_BuildSettings broadcast
//      (settingsbuilder.h),
//   3. Lua via MCM.RegisterBuildSettingsListener callbacks (scriptbind.h).
// No persistence — values live in g_mods for the session (Snapshot/ApplyValues
// carry them across rebuilds); consuming plugins own their own storage
// (see api/MCM_API.h).

#pragma once

#include <algorithm>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "pugixml.hpp"

#include "Offsets/vtables/ICryPak.h"
#include "mcm.h"

struct Item {
    enum Kind { Cat, Toggle, Dropdown, Slider } kind = Cat;
    std::string id, label, tip;
    std::string options;                 // dropdown: "A|B|C"
    std::string suffix;                  // slider: shown verbatim ("" / "%" / "s" ...)
    double mn = 0, mx = 1, step = 1;
    double def = 0;                      // toggle 0/1, dropdown index, slider value
    double val = 0;
    bool hasLiveValue = false;           // true if SetItemValue reported val THIS rebuild
                                          // (ApplyValues must not clobber it with a stale snapshot)
};

struct Mod {
    std::string id, name;
    std::vector<Item> items;
};

inline std::vector<Mod> g_mods;

// Find-or-create by modId. A non-empty displayName overwrites a placeholder
// name (mods auto-created by an Add* call before their AddMod get name = id).
inline Mod& GetOrAddMod(const std::string& id, const std::string& name)
{
    for (Mod& m : g_mods)
        if (m.id == id) {
            if (!name.empty() && name != id)
                m.name = name;
            return m;
        }
    Mod m;
    m.id   = id;
    m.name = name.empty() ? id : name;
    g_mods.push_back(std::move(m));
    return g_mods.back();
}

// First definition of a setting id wins (categories carry no id and always
// append).
inline void AddItem(Mod& mod, Item&& it)
{
    if (it.kind != Item::Cat)
        for (const Item& e : mod.items)
            if (e.kind != Item::Cat && e.id == it.id)
                return;
    mod.items.push_back(std::move(it));
}

// Overwrites an already-added item's DISPLAYED value, leaving its default
// alone (build-callback only; the item must already exist -- call this right
// after the matching Add* for the same id). For a setting whose live value
// can differ from its schema default (e.g. it mirrors a CVar or other
// externally-owned state), this is how a builder reports "this is what's
// live right now" without losing the X-reset target. Returns false (does
// nothing) if no such item exists yet.
inline bool SetItemValue(Mod& mod, const std::string& id, double value)
{
    for (Item& it : mod.items)
        if (it.kind != Item::Cat && it.id == id) {
            it.val = value;
            it.hasLiveValue = true;
            return true;
        }
    return false;
}

// Session values keyed by (modId, settingId), carried across rebuilds.
using ValueSnapshot = std::map<std::pair<std::string, std::string>, double>;

inline ValueSnapshot SnapshotValues()
{
    ValueSnapshot snap;
    for (const Mod& m : g_mods)
        for (const Item& it : m.items)
            if (it.kind != Item::Cat)
                snap[{ m.id, it.id }] = it.val;
    return snap;
}

// Restores in-menu edits across a rebuild for items nobody else re-asserted
// a live value for THIS round (see SetItemValue) -- MCM is the only owner of
// state for those, so without this every rebuild would reset them to their
// Add* default.
inline void ApplyValues(const ValueSnapshot& snap)
{
    for (Mod& m : g_mods)
        for (Item& it : m.items) {
            if (it.kind == Item::Cat || it.hasLiveValue)
                continue;
            auto f = snap.find({ m.id, it.id });
            if (f != snap.end())
                it.val = f->second;
        }
}

inline double ToNum(const char* s, double fallback)
{
    if (!s || !*s)
        return fallback;
    if (strcmp(s, "true") == 0 || strcmp(s, "True") == 0)
        return 1;
    if (strcmp(s, "false") == 0 || strcmp(s, "False") == 0)
        return 0;
    try {
        return std::stod(s);
    } catch (...) {
        return fallback;
    }
}

inline void ParseItems(const pugi::xml_node& parent, std::vector<Item>& out)
{
    for (pugi::xml_node n : parent.children()) {
        Item it;
        it.id    = n.attribute("id").value();
        it.label = n.attribute("label").value();
        it.tip   = n.attribute("tooltip").value();
        if (strcmp(n.name(), "Category") == 0) {
            it.kind = Item::Cat;
            out.push_back(it);
            ParseItems(n, out);          // items may nest inside a category
            continue;
        }
        if (strcmp(n.name(), "Toggle") == 0) {
            it.kind = Item::Toggle;
            it.def = ToNum(n.attribute("default").value(), 0) != 0 ? 1 : 0;
        } else if (strcmp(n.name(), "Dropdown") == 0) {
            it.kind = Item::Dropdown;
            it.options = n.attribute("options").value();
            it.def = ToNum(n.attribute("default").value(), 0);
        } else if (strcmp(n.name(), "Slider") == 0) {
            it.kind = Item::Slider;
            it.mn   = ToNum(n.attribute("min").value(), 0);
            it.mx   = ToNum(n.attribute("max").value(), 1);
            it.step = ToNum(n.attribute("step").value(), 1);
            it.def  = ToNum(n.attribute("default").value(), it.mn);
            it.suffix = n.attribute("suffix").value();
        } else {
            continue;                    // unknown tag — skip
        }
        if (it.id.empty() || it.label.empty()) {
            MCM_LOG("WARN: <%s> without id/label skipped", n.name());
            continue;
        }
        it.val = it.def;
        out.push_back(it);
    }
}

// Reads one file out of the pak filesystem; empty string on failure.
inline std::string ReadPakFile(Offsets::ICryPak* pak, const char* path)
{
    void* f = pak->FOpen(path, "rb", 0);
    if (!f)
        return {};
    std::string buf(pak->FGetSize(f), '\0');
    size_t got = buf.empty() ? 0 : pak->FReadRawAll(buf.data(), buf.size(), f);
    pak->FClose(f);
    buf.resize(got);
    return buf;
}

// Scans Libs/UI/MCM/*.xml across all mounted paks and merges the mods found
// into g_mods (append-only; RebuildSettings owns clear/carry/sort).
inline void ScanConfigXml()
{
    auto* env = SSystemGlobalEnvironment::GetInstance();
    Offsets::ICryPak* pak = env ? env->pCryPak : nullptr;
    if (!pak) {
        MCM_LOG("ERROR: CryPak unavailable - no xml configs loaded");
        return;
    }
    Offsets::SCryPakFindData fd{};
    intptr_t h = pak->FindFirst("Libs/UI/MCM/*.xml", &fd, 0, false);
    if (h > -1) {
        do {
            std::string path = std::string("Libs/UI/MCM/") + fd.name;
            std::string xml = ReadPakFile(pak, path.c_str());
            pugi::xml_document doc;
            pugi::xml_node root;
            if (xml.empty() ||
                !doc.load_buffer(xml.data(), xml.size()) ||
                !(root = doc.child("MCMConfig"))) {
                MCM_LOG("WARN: %s is not a valid MCMConfig", path.c_str());
                continue;
            }
            std::string id = root.attribute("modid").value();
            if (id.empty()) {
                id = fd.name;                            // filename stem fallback
                size_t dot = id.rfind('.');
                if (dot != std::string::npos)
                    id.resize(dot);
            }
            std::vector<Item> items;
            ParseItems(root, items);
            Mod& mod = GetOrAddMod(id, root.attribute("name").value());
            for (Item& it : items)
                AddItem(mod, std::move(it));
        } while (pak->FindNext(h, &fd) >= 0);
        pak->FindClose(h);
    }
}
