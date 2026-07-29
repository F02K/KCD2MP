#include <MinHook.h>

#include "KCSE/KCSEAPI.h"
#include "REL.h"

#include "playermodule/C_Keybinds.h"
#include "playermodule/E_KeybindController.h"

namespace {

// C_Keybinds::HasConflicts -- the settings-page Apply/Back commit gates' only conflict
class{
public:
    static bool Install() {
        void* target = reinterpret_cast<void*>(REL::ID(102).address());
        if (MH_CreateHook(target, reinterpret_cast<void*>(&thunk),
            reinterpret_cast<void**>(&orig)) != MH_OK)
            return false;

        return MH_EnableHook(target) == MH_OK;
    }

    static void Uninstall() {
        void* target = reinterpret_cast<void*>(REL::ID(102).address());
        MH_RemoveHook(target);
    }

protected:
    static bool thunk(wh::playermodule::C_Keybinds* self,
                      wh::playermodule::E_KeybindController::Type controller) {
        return false;
    }

    static inline REL::Relocation<decltype(&thunk)> orig;
}hkHasConflicts;

}  // namespace

bool InstallHooks() {
    if (MH_Initialize() != MH_OK)
        return false;

    return hkHasConflicts.Install();
}

KCSE_PLUGIN_INFO("Allow Conflicting Keybinds", "JerryYOJ", 1);
KCSE_PLUGIN_LOAD(kcse)
{
    return InstallHooks();
}
