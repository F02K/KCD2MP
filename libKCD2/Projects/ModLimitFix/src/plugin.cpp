#include <MinHook.h>

#include "KCSE/KCSEAPI.h"
#include "REL.h"

namespace {

// Plaion telemetry's ModsInfo builder. Building one detailed object per enabled
// mod exhausts DSOnline's fixed arena; the data is optional and not used by the game.
class {
public:
    static bool Install()
    {
        void* target = reinterpret_cast<void*>(REL::ID(202646).address());
        if (MH_CreateHook(target, reinterpret_cast<void*>(&BuildModsInfo),
            reinterpret_cast<void**>(&orig)) != MH_OK)
            return false;

        return MH_EnableHook(target) == MH_OK;
    }

    static void Uninstall()
    {
        void* target = reinterpret_cast<void*>(REL::ID(202646).address());
        MH_RemoveHook(target);
    }

protected:
    static void BuildModsInfo(void*) {}

    static inline REL::Relocation<decltype(&BuildModsInfo)> orig;
} hkBuildModsInfo;

}  // namespace

bool InstallHooks()
{
    if (MH_Initialize() != MH_OK)
        return false;

    return hkBuildModsInfo.Install();
}

KCSE_PLUGIN_INFO("ModLimitFix", "JerryYOJ", 1);
KCSE_PLUGIN_LOAD(kcse)
{
    return InstallHooks();
}
