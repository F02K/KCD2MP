#pragma once

#include "AlchemyStart/AlchemyStart.h"
#include "AlchemyEnd/AlchemyEnd.h"
#include "BrewResult/BrewResult.h"

namespace hooks {

inline bool Install()
{
    return AlchemyStartHook::Install()
        && AlchemyEndHook::Install()
        && BrewResultHook::Install();
}

inline void Uninstall()
{
    BrewResultHook::Uninstall();
    AlchemyEndHook::Uninstall();
    AlchemyStartHook::Uninstall();
}

}  // namespace hooks
