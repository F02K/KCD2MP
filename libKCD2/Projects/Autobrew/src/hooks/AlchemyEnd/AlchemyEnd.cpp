#include "AlchemyEnd.h"

#include <MinHook.h>

#include "Autobrew.h"

namespace hooks {

bool AlchemyEndHook::Install()
{
    void* target = reinterpret_cast<void*>(REL::ID(54603).address());
    if (MH_CreateHook(target, reinterpret_cast<void*>(&AlchemyEnd),
                      reinterpret_cast<void**>(&orig)) != MH_OK)
        return false;

    return MH_EnableHook(target) == MH_OK;
}

void AlchemyEndHook::Uninstall()
{
    MH_RemoveHook(reinterpret_cast<void*>(REL::ID(54603).address()));
}

uint64_t AlchemyEndHook::AlchemyEnd(wh::playermodule::C_Alchemy* self)
{
    Autobrew::OnSessionEnd(self);
    return orig(self);
}

}  // namespace hooks
