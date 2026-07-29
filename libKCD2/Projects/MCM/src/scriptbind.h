// CScriptBind_MCM - Lua-facing MCM scriptbind (CryEngine ScriptBind idiom).
//
//   MCM.RegisterBuildSettingsListener( function() ... end )
//   MCM.AddMod( modId, displayName )                                -- build-callback only
//   MCM.AddCategory( modId, label, tooltip )                        -- build-callback only
//   MCM.AddToggle( modId, id, label, tooltip, default01 )           -- build-callback only
//   MCM.AddDropdown( modId, id, label, tooltip, options, default )  -- build-callback only
//   MCM.AddSlider( modId, id, label, tooltip, min, max, step, default, suffix )
//   MCM.SetValue( modId, id, value )                                 -- build-callback only
//   MCM.RegisterValueChangeListener( modId, function(settingId, value) ... end )
//
// Build listeners fire on every settings rebuild (each menu open) and
// contribute settings by calling the MCM.Add* functions imperatively — those
// are valid ONLY inside the callback (the model is cleared and rebuilt each
// time, so re-adding is the contract, and session values are re-applied by id
// afterwards). SetValue overwrites an already-added item's DISPLAYED value
// without touching its default — call it right after the matching Add* when
// your setting's live value can differ from its schema default (e.g. it
// mirrors a CVar you own); an X-reset in the menu then restores the Add*
// default, not whatever was live. Skip it for settings MCM itself is the
// only owner of — those already carry session edits across rebuilds on
// their own. Value-change callbacks fire on every value change of their
// mod, at the same moment the KCSE ValueChanged broadcast goes out
// (api/MCM_API.h is the C++ counterpart). Dot-call only - colon-call shifts
// the args by one (self) and raises a param-type script error.
// Re-registering the same closure replaces the old entry (script reloads
// re-run init code). Values arrive as floats (the VM is float-Lua).
//
// Registration is engine-side: IScriptTable::AddFunction with a member functor
// (functor.h), the same descriptor idiom the game's own binds use (see
// crysystem/CScriptableBase.h); the global is published via SetGlobalAny.

#pragma once

#include <string>
#include <vector>

#include "crysystem/SSystemGlobalEnvironment.h"
#include "crysystem/ScriptAnyValue.h"
#include "crysystem/SUserFunctionDesc.h"
#include "Offsets/vtables/IFunctionHandler.h"
#include "Offsets/vtables/IScriptSystem.h"
#include "Offsets/vtables/IScriptTable.h"
#include "configmanager.h"
#include "mcm.h"

class CScriptBind_MCM
{
public:
    // Engine-side equivalent: CScriptableBase::Init + SetGlobalName.
    void Init(Offsets::IScriptSystem* pSS)
    {
        m_pSS = pSS;
        m_pMethodsTable = pSS->CreateTable(0, 0);
        if (!m_pMethodsTable) {
            MCM_LOG("ERROR: CreateTable failed - no lua API");
            return;
        }
        m_pMethodsTable->AddRef();               // CreateTable returns refcount 0; pinned for the session

        RegisterFunction("RegisterValueChangeListener", "modId, callback",
                         functor(*this, &CScriptBind_MCM::RegisterValueChangeListener));
        RegisterFunction("RegisterBuildSettingsListener", "callback",
                         functor(*this, &CScriptBind_MCM::RegisterBuildSettingsListener));
        RegisterFunction("AddMod", "modId, displayName",
                         functor(*this, &CScriptBind_MCM::AddMod));
        RegisterFunction("AddCategory", "modId, label, tooltip",
                         functor(*this, &CScriptBind_MCM::AddCategory));
        RegisterFunction("AddToggle", "modId, id, label, tooltip, default",
                         functor(*this, &CScriptBind_MCM::AddToggle));
        RegisterFunction("AddDropdown", "modId, id, label, tooltip, options, default",
                         functor(*this, &CScriptBind_MCM::AddDropdown));
        RegisterFunction("AddSlider", "modId, id, label, tooltip, min, max, step, default, suffix",
                         functor(*this, &CScriptBind_MCM::AddSlider));
        RegisterFunction("SetValue", "modId, id, value",
                         functor(*this, &CScriptBind_MCM::SetValue));

        m_pSS->SetGlobalAny("MCM", ScriptAnyValue(m_pMethodsTable));
        MCM_LOG("lua scriptbind ready (MCM.*)");
    }

    bool IsInitialized() const { return m_pMethodsTable != nullptr; }

    //! MCM.RegisterValueChangeListener( modId, callback )
    int RegisterValueChangeListener(Offsets::IFunctionHandler* pH)
    {
        const char* modId = nullptr;
        HSCRIPTFUNCTION callback = nullptr;      // typed GetParam refs the closure - the ref is ours
        if (!pH->GetParams(modId, callback) || !modId)   // mismatch raises a script error
            return pH->EndFunction();

        for (SLuaListener& l : m_valueListeners) {
            if (l.modId == modId && m_pSS->CompareFuncRef(l.fn, callback)) {
                m_pSS->ReleaseFunc(l.fn);        // same closure re-registered: swap refs
                l.fn = callback;
                return pH->EndFunction();
            }
        }
        m_valueListeners.push_back({ modId, callback });
        MCM_LOG("lua value listener registered for '%s' (%zu total)", modId, m_valueListeners.size());
        return pH->EndFunction();
    }

    //! MCM.RegisterBuildSettingsListener( callback )
    int RegisterBuildSettingsListener(Offsets::IFunctionHandler* pH)
    {
        HSCRIPTFUNCTION callback = nullptr;
        if (!pH->GetParams(callback))
            return pH->EndFunction();

        for (HSCRIPTFUNCTION& fn : m_buildListeners) {
            if (m_pSS->CompareFuncRef(fn, callback)) {
                m_pSS->ReleaseFunc(fn);
                fn = callback;
                return pH->EndFunction();
            }
        }
        m_buildListeners.push_back(callback);
        MCM_LOG("lua build listener registered (%zu total)", m_buildListeners.size());
        return pH->EndFunction();
    }

    //! MCM.AddMod( modId, displayName )
    int AddMod(Offsets::IFunctionHandler* pH)
    {
        const char* modId = nullptr;
        const char* name = nullptr;
        if (Building(pH) && pH->GetParams(modId, name) && modId && *modId)
            GetOrAddMod(modId, name ? name : "");
        return pH->EndFunction();
    }

    //! MCM.AddCategory( modId, label, tooltip )
    int AddCategory(Offsets::IFunctionHandler* pH)
    {
        const char* modId = nullptr;
        const char* label = nullptr;
        const char* tip = nullptr;
        if (!Building(pH) || !pH->GetParams(modId, label, tip) || !*modId || !*label)
            return pH->EndFunction();
        Item it;
        it.kind  = Item::Cat;
        it.label = label;
        it.tip   = tip;
        AddItem(GetOrAddMod(modId, ""), std::move(it));
        return pH->EndFunction();
    }

    //! MCM.AddToggle( modId, id, label, tooltip, default01 )
    int AddToggle(Offsets::IFunctionHandler* pH)
    {
        const char* modId = nullptr;
        const char* id = nullptr;
        const char* label = nullptr;
        const char* tip = nullptr;
        double def = 0;
        if (!Building(pH) || !pH->GetParams(modId, id, label, tip, def) || !*modId || !*id || !*label)
            return pH->EndFunction();
        Item it;
        it.kind  = Item::Toggle;
        it.id    = id;
        it.label = label;
        it.tip   = tip;
        it.def   = it.val = (def != 0) ? 1 : 0;
        AddItem(GetOrAddMod(modId, ""), std::move(it));
        return pH->EndFunction();
    }

    //! MCM.AddDropdown( modId, id, label, tooltip, options "A|B|C", defaultIndex )
    int AddDropdown(Offsets::IFunctionHandler* pH)
    {
        const char* modId = nullptr;
        const char* id = nullptr;
        const char* label = nullptr;
        const char* tip = nullptr;
        const char* options = nullptr;
        double def = 0;
        if (!Building(pH) || !pH->GetParams(modId, id, label, tip, options, def) ||
            !*modId || !*id || !*label || !*options)
            return pH->EndFunction();
        Item it;
        it.kind    = Item::Dropdown;
        it.id      = id;
        it.label   = label;
        it.tip     = tip;
        it.options = options;
        it.def     = it.val = (int)def;
        AddItem(GetOrAddMod(modId, ""), std::move(it));
        return pH->EndFunction();
    }

    //! MCM.AddSlider( modId, id, label, tooltip, min, max, step, default, suffix )
    int AddSlider(Offsets::IFunctionHandler* pH)
    {
        const char* modId = nullptr;
        const char* id = nullptr;
        const char* label = nullptr;
        const char* tip = nullptr;
        double mn = 0, mx = 1, step = 1, def = 0;
        const char* suffix = nullptr;
        if (!Building(pH) || !pH->GetParams(modId, id, label, tip, mn, mx, step, def, suffix) ||
            !*modId || !*id || !*label)
            return pH->EndFunction();
        Item it;
        it.kind   = Item::Slider;
        it.id     = id;
        it.label  = label;
        it.tip    = tip;
        it.mn     = mn;
        it.mx     = mx;
        it.step   = step;
        it.def    = it.val = def;
        it.suffix = suffix;
        AddItem(GetOrAddMod(modId, ""), std::move(it));
        return pH->EndFunction();
    }

    //! MCM.SetValue( modId, id, value )
    int SetValue(Offsets::IFunctionHandler* pH)
    {
        const char* modId = nullptr;
        const char* id = nullptr;
        double value = 0;
        if (!Building(pH) || !pH->GetParams(modId, id, value) || !*modId || !*id)
            return pH->EndFunction();
        if (!SetItemValue(GetOrAddMod(modId, ""), id, value))
            MCM_LOG("WARN: MCM.SetValue(%s.%s) - no such setting (call after its Add*)", modId, id);
        return pH->EndFunction();
    }

    // Runs the registered build callbacks (RebuildSettings, plugin.cpp); the
    // MCM.Add* functions are unlocked only for their duration.
    void NotifyBuildSettings()
    {
        if (m_buildListeners.empty())
            return;
        m_building = true;
        for (HSCRIPTFUNCTION fn : m_buildListeners) {
            if (m_pSS->BeginCall(fn))
                m_pSS->EndCall();
        }
        m_building = false;
    }

    // Value-change fan-out to this mod's Lua callbacks: cb(settingId, value).
    void OnValueChanged(const char* modId, const char* settingId, double value)
    {
        for (const SLuaListener& l : m_valueListeners) {
            if (l.modId != modId)
                continue;
            if (!m_pSS->BeginCall(l.fn))
                continue;
            m_pSS->PushFuncParamAny(ScriptAnyValue(settingId));
            m_pSS->PushFuncParamAny(ScriptAnyValue(value));
            m_pSS->EndCall();
        }
    }

private:
    void RegisterFunction(const char* sFuncName, const char* sFuncParams, const FunctionFunctor& f)
    {
        SUserFunctionDesc fd;
        fd.sGlobalName     = "MCM";
        fd.sFunctionName   = sFuncName;
        fd.sFunctionParams = sFuncParams;
        fd.pFunctor        = f;
        m_pMethodsTable->AddFunction(fd);
    }

    bool Building(Offsets::IFunctionHandler* pH) const
    {
        if (!m_building)
            MCM_LOG("WARN: MCM.%s outside a BuildSettings callback - ignored", pH->GetFuncName());
        return m_building;
    }

    struct SLuaListener {
        std::string     modId;
        HSCRIPTFUNCTION fn;      // owned Lua registry ref (released only when replaced)
    };

    Offsets::IScriptSystem*      m_pSS = nullptr;
    Offsets::IScriptTable*       m_pMethodsTable = nullptr;
    std::vector<SLuaListener>    m_valueListeners;
    std::vector<HSCRIPTFUNCTION> m_buildListeners;
    bool                         m_building = false;
};

inline CScriptBind_MCM g_scriptBindMCM;

// ------------------------------------------------------------ plugin glue ---

inline void InitScriptBind()
{
    if (auto* env = SSystemGlobalEnvironment::GetInstance(); env && env->pScriptSystem)
        g_scriptBindMCM.Init(env->pScriptSystem);
}

inline void NotifyLuaBuildSettings()
{
    g_scriptBindMCM.NotifyBuildSettings();
}

inline void NotifyLua(const char* modId, const char* settingId, double value)
{
    g_scriptBindMCM.OnValueChanged(modId, settingId, value);
}
