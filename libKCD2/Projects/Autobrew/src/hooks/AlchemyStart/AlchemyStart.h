#pragma once

#include "REL.h"
#include "playermodule/C_Alchemy.h"

namespace hooks {

// C_Alchemy::Start (0x180897E1C, REL::ID 47131) -- runs when a brewing session binds to a table
// and pushes the "alchemy" input context.  Post-call (success only): register the Auto Brew prompt.
class AlchemyStartHook {
public:
    static bool Install();
    static void Uninstall();

private:
    static char AlchemyStart(wh::playermodule::C_Alchemy* self, EntityId tableEntityId);

    inline static REL::Relocation<decltype(&AlchemyStart)> orig;
};

}  // namespace hooks
