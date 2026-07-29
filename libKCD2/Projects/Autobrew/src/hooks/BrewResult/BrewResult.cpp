#include "BrewResult.h"

#include <MinHook.h>

#include "Autobrew.h"

namespace hooks {

bool BrewResultHook::Install()
{
    void* target = reinterpret_cast<void*>(REL::ID(375581).address());
    if (MH_CreateHook(target, reinterpret_cast<void*>(&PresentResult),
                      reinterpret_cast<void**>(&orig)) != MH_OK)
        return false;

    return MH_EnableHook(target) == MH_OK;
}

void BrewResultHook::Uninstall()
{
    MH_RemoveHook(reinterpret_cast<void*>(REL::ID(375581).address()));
}

void BrewResultHook::PresentResult(wh::playermodule::C_Alchemy* self, char code,
                                   wh::entitymodule::C_Item* item, uint32_t count)
{
    if (Autobrew::OnBrewResult(self, code, item, count))
        return;   // batch verdict toasted -- the m_onBrewResult emit (and its modal) skipped
    orig(self, code, item, count);
}

}  // namespace hooks
