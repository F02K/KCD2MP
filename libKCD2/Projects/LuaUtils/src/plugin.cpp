// LuaUtils - KCSE plugin exposing the item/equipment Lua API the vanilla
// surface lacks (per-instance equip status, item stats, sanctioned setters).
//
// Registration:
//  - ItemManager (vanilla global, extended): MinHook on the C_ScriptBindItemManager
//    fn-table registrar - after the original registers the 7 vanilla fns, our
//    descriptors are added into the same methods table (CScriptableBase+0x48).
//    KCSE loads plugins from the dinput8 proxy at process start, before the
//    bind factory runs, so the hook always catches the registration.
//  - EquipmentManager (new global): MCM SetGlobalAny pattern at PreDataLoaded.

#include <MinHook.h>

#include "KCSE/KCSEAPI.h"
#include "REL.h"
#include "crysystem/CScriptableBase.h"
#include "crysystem/SSystemGlobalEnvironment.h"
#include "entitymodule/C_ScriptBindItemManager.h"
#include "scriptbind/ScriptBind_EquipmentManager.h"
#include "scriptbind/ScriptBind_ItemManager.h"

// ------------------------------------------------------------------ hooks ---

namespace {

    class {
    public:
        static bool Install() {
            void* target = reinterpret_cast<void*>(REL::ID(104708).address());  // 0x1812B2D38
            if (MH_CreateHook(target, reinterpret_cast<void*>(&RegisterFunctions),
                              reinterpret_cast<void**>(&orig)) != MH_OK)
                return false;

            return MH_EnableHook(target) == MH_OK;
        }

    protected:
        static void RegisterFunctions(wh::entitymodule::C_ScriptBindItemManager* self) {
            orig(self);
            luautils::g_itemManagerExt.Attach(self->m_pSS, self->m_pMethodsTable);
        }

        static inline REL::Relocation<decltype(&RegisterFunctions)> orig;
    } hkItemManagerRegisterFunctions;

}  // namespace

// -------------------------------------------------------------- KCSE glue ---

static void InitScriptBinds()
{
    if (auto* env = SSystemGlobalEnvironment::GetInstance(); env && env->pScriptSystem) {
        if (!luautils::g_equipmentManagerBind.IsInitialized())
            luautils::g_equipmentManagerBind.Init(env->pScriptSystem);
    }
}

KCSE_PLUGIN_INFO("LuaUtils", "JerryYOJ", 1);

KCSE_PLUGIN_LOAD(kcse)
{
    if (MH_Initialize() != MH_OK)
        return false;
    if (!hkItemManagerRegisterFunctions.Install())
        return false;

    kcse->GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) {
        if (msg->type == KCSE::IMessagingInterface::kMessage_PreDataLoaded)
            InitScriptBinds();
    });
    return true;
}
