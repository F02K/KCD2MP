// Hook sites:
//   defaultActionHelp.xml    write_call<5> @ REL::ID 65542 + 0x48 wrapping C_ActionSets::ParseActionHelp_180A13558
//                            (caller 0x180C3D5B4 is an I_ActionSets vfunc: it loads the XML and
//                            calls the parser with this-8 = the complete C_ActionSets object)
//   keybindSuperactions.xml  write_call<5> @ REL::ID 33537 + 0x73 wrapping C_Keybinds::ParseKeybindsXml_1808FB4CC
//   defaultProfile.xml       MinHook on CActionMapManager::LoadFromXML_180A13E3C (REL::ID 55292,
//                            IActionMapManager slot [7]). Two verified invokers:
//                            C_Game::LoadConfigXml_18173CB3C (stock profile path, overridable)
//                            and InitActionMaps_183573BFC (arbitrary file, but it records it
//                            via SetLoadFromXMLPath first) — so the detour keys the patch by
//                            GetLoadFromXMLPath() and falls back to the stock path when unset.
//                            Controller layouts and user rebinds go through the separate
//                            LoadRebindDataFromXML slot [8] and are never touched.

#include <MinHook.h>

#include "KCSE/KCSEAPI.h"
#include "REL.h"
#include "crysystem/SSystemGlobalEnvironment.h"
#include "Offsets/vtables/IActionMapManager.h"
#include "Offsets/vtables/ICryPak.h"
#include "Offsets/vtables/ILog.h"
#include "Offsets/vtables/ISystem.h"
#include "Offsets/vtables/IXmlNode.h"
#include "playermodule/C_ActionSets.h"
#include "playermodule/C_Keybinds.h"

#include <cstring>
#include <string>

using Offsets::IXmlNode;

// --------------------------------------------------------------- PTF merge ---

namespace {

// IXmlNode::getAttr [33] returns the engine's static "" when absent (never
// NULL); the guard is belt-and-suspenders.
const char* AttrOrEmpty(IXmlNode* node, const char* key)
{
    const char* v = node->getAttr(key);
    return v ? v : "";
}

void CopyAttributes(IXmlNode* dst, IXmlNode* src)
{
    const int n = src->getNumAttributes();
    for (int i = 0; i < n; ++i) {
        const char* key = nullptr;
        const char* val = nullptr;
        if (src->getAttributeByIndex(i, &key, &val) && key && val)
            dst->setAttr(key, val);
    }
}

// Row identity = tag + the identifying attributes, absent attrs compare as "".
// The config XML families use different id attrs: <actionmap name=...>,
// <action_help_set actionmap=... [alternative=...]>, <hint action=...>, and in
// keybindSuperactions <action name=... map=...> / <control input=... controller=...>
// — so equality spans all of them instead of hardcoding one key per file.
bool SameIdentity(IXmlNode* a, IXmlNode* b)
{
    if (std::strcmp(a->getTag(), b->getTag()) != 0)
        return false;
    static const char* const kIdAttrs[] = { "name", "actionmap", "action", "alternative",
                                            "map", "input", "controller" };
    for (const char* key : kIdAttrs)
        if (std::strcmp(AttrOrEmpty(a, key), AttrOrEmpty(b, key)) != 0)
            return false;
    return true;
}

int FindChildIndex(IXmlNode* base, IXmlNode* like)
{
    const int n = base->getChildCount();
    for (int i = 0; i < n; ++i) {
        IXmlNode* c = nullptr;
        base->getChild(&c, i);
        if (!c)
            continue;
        const bool match = SameIdentity(c, like);
        c->Release();
        if (match)
            return i;
    }
    return -1;
}

struct S_MergeStats {
    int added = 0;      // appended nodes (no key match in base), any depth
    int modified = 0;   // replaced leaves + attr-updated containers, any depth
};

// Stock row-merge, transplanted to XML: keyed replace-or-append, recursing into
// containers so a mod can patch a single <action> without restating the whole
// actionmap. Patch subtrees are shared into the base document — the same
// technique the stock XMLPatcher uses (insertChild/replaceChild AddRef the node).
void MergeChildren(IXmlNode* base, IXmlNode* patch, S_MergeStats& stats)
{
    const int n = patch->getChildCount();
    for (int i = 0; i < n; ++i) {
        IXmlNode* p = nullptr;
        patch->getChild(&p, i);
        if (!p)
            continue;

        const int at = FindChildIndex(base, p);
        if (at < 0) {
            IXmlNode* add = nullptr;                         // clone: p may be read-only
            p->clone(&add, false);
            if (add) {
                base->insertChild(base->getChildCount(), &add);  // added
                add->Release();
                ++stats.added;
            }
        } else {
            IXmlNode* b = nullptr;
            base->getChild(&b, at);
            if (b) {
                if (p->getChildCount() == 0 && b->getChildCount() == 0) {
                    IXmlNode* repl = nullptr;
                    p->clone(&repl, false);
                    if (repl) {
                        base->replaceChild(at, &repl);       // modified (leaf row)
                        repl->Release();
                        ++stats.modified;
                    }
                } else {
                    // container (either side has children): never replace — a
                    // patch leaf must not wipe the base node's subtree
                    if (p->getNumAttributes() > 0) {
                        CopyAttributes(b, p);                // modified (attrs)
                        ++stats.modified;
                    }
                    MergeChildren(b, p, stats);              // recurse
                }
                b->Release();
            }
        }
        p->Release();
    }
}

// Merge every <stem>__<modid><ext> patch the pak FS has for basePath into the
// tree at *ppBaseRoot. The base tree is a read-only pooled one, so patches land
// in a mutable deep clone and *ppBaseRoot is swapped to it (old ref released).
// Returns the number of patch files applied.
int ApplyPtfPatches(const char* basePath, IXmlNode** ppBaseRoot)
{
    if (!basePath || !ppBaseRoot || !*ppBaseRoot)
        return 0;

    auto* env = SSystemGlobalEnvironment::GetInstance();
    if (!env || !env->pCryPak || !env->pSystem)
        return 0;

    // "libs/config/foo.xml" -> "libs/config/foo__*.xml"
    std::string wildcard(basePath);
    const size_t dot = wildcard.rfind('.');
    if (dot == std::string::npos)
        return 0;
    wildcard.insert(dot, "__*");

    // find records carry bare file names; keep the directory for reloading
    const size_t slash = wildcard.find_last_of("/\\");
    const std::string dir =
        (slash == std::string::npos) ? std::string() : wildcard.substr(0, slash + 1);

    Offsets::SCryPakFindData fd{};
    const intptr_t h = env->pCryPak->FindFirst(wildcard.c_str(), &fd);
    if (h == -1)
        return 0;

    IXmlNode* merged = nullptr;
    (*ppBaseRoot)->clone(&merged, false);
    if (!merged) {
        env->pCryPak->FindClose(h);
        return 0;
    }

    int applied = 0;
    do {
        const std::string patchPath = dir + fd.name;
        IXmlNode* patchRoot = nullptr;
        // same call shape as the three stock config loaders: (&out, path, 0, 1, 1)
        env->pSystem->LoadXmlFromFile(&patchRoot, patchPath.c_str(), 0, 1, 1);
        if (!patchRoot) {
            if (env->pLog)
                env->pLog->LogAlways("[PTFextender] Failed to load patch '%s'", patchPath.c_str());
            continue;
        }
        S_MergeStats stats;
        MergeChildren(merged, patchRoot, stats);
        patchRoot->Release();
        ++applied;
        if (env->pLog)
            env->pLog->LogAlways("[PTFextender] '%s' is patched by '%s', nodes added: %d, modified: %d",
                                 basePath, fd.name, stats.added, stats.modified);
    } while (env->pCryPak->FindNext(h, &fd) >= 0);
    env->pCryPak->FindClose(h);

    if (applied > 0) {
        (*ppBaseRoot)->Release();
        *ppBaseRoot = merged;      // caller's XmlNodeRef now owns the merged tree
    } else {
        merged->Release();
    }
    return applied;
}

}  // namespace

// ------------------------------------------------ hook: defaultActionHelp ---

class {
public:
    static bool Install() {
        orig = REL::Relocation<>{ REL::ID(65542), 0x48 }.write_call<5>(ParseActionHelp);
        return orig.address();
    }

protected:
    static void ParseActionHelp(wh::playermodule::C_ActionSets* pThis, IXmlNode** ppXml) {
        if (ppXml && *ppXml)
            ApplyPtfPatches("libs/config/defaultActionHelp.xml", ppXml);
        orig(pThis, ppXml);
    }

    static inline REL::Relocation<decltype(&ParseActionHelp)> orig;
} hkParseActionHelp;

// ---------------------------------------------- hook: keybindSuperactions ---

class {
public:
    static bool Install() {
        orig = REL::Relocation<>{ REL::ID(33537), 0x73 }.write_call<5>(ParseKeybindsXml);
        return orig.address();
    }

protected:
    static void ParseKeybindsXml(wh::playermodule::C_Keybinds* pThis, IXmlNode** ppXml) {
        if (ppXml && *ppXml)
            ApplyPtfPatches("libs/config/keybindSuperactions.xml", ppXml);
        orig(pThis, ppXml);
    }

    static inline REL::Relocation<decltype(&ParseKeybindsXml)> orig;
} hkParseKeybindsXml;

// -------------------------------------------------- hook: defaultProfile ---

class {
public:
    static bool Install() {
        void* target = reinterpret_cast<void*>(REL::ID(55292).address());
        if (MH_CreateHook(target, reinterpret_cast<void*>(&LoadFromXML),
                          reinterpret_cast<void**>(&orig)) != MH_OK)
            return false;

        return MH_EnableHook(target) == MH_OK;
    }

protected:
    static char LoadFromXML(Offsets::IActionMapManager* pThis, IXmlNode** ppXml) {
        if (ppXml && *ppXml) {
            // InitActionMaps records its source file (SetLoadFromXMLPath) before
            // invoking this parser; the C_Game::LoadConfigXml path does not, so
            // an empty path means the stock profile file.
            const char* path = pThis->GetLoadFromXMLPath();
            if (!path || !*path)
                path = "libs/config/defaultProfile.xml";
            ApplyPtfPatches(path, ppXml);
        }
        return orig(pThis, ppXml);
    }

    static inline REL::Relocation<decltype(&LoadFromXML)> orig;
} hkLoadFromXML;

// ---------------------------------------------------------------- install ---

static bool InstallHooks()
{
    if (MH_Initialize() != MH_OK)
        return false;

    return hkParseActionHelp.Install()
        && hkParseKeybindsXml.Install()
        && hkLoadFromXML.Install();
}

// -------------------------------------------------------------- KCSE glue ---

KCSE_PLUGIN_INFO("PTF Extender", "JerryYOJ", 1);

KCSE_PLUGIN_LOAD(kcse)
{
    KCSE::AllocTrampoline(1 << 10);
    
    return InstallHooks();
}
