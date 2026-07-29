#include "AlchemyStart.h"

#include <MinHook.h>

#include "Autobrew.h"

namespace hooks {

bool AlchemyStartHook::Install()
{
    void* target = reinterpret_cast<void*>(REL::ID(47131).address());
    if (MH_CreateHook(target, reinterpret_cast<void*>(&AlchemyStart),
                      reinterpret_cast<void**>(&orig)) != MH_OK)
        return false;

    return MH_EnableHook(target) == MH_OK;
}

void AlchemyStartHook::Uninstall()
{
    MH_RemoveHook(reinterpret_cast<void*>(REL::ID(47131).address()));
}

char AlchemyStartHook::AlchemyStart(wh::playermodule::C_Alchemy* self, uint32_t tableEntityId)
{
    const char ok = orig(self, tableEntityId);
    if (ok)
        Autobrew::OnSessionStart(self);
    return ok;
}

}  // namespace hooks
