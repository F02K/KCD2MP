#pragma once

#include "REL.h"
#include "playermodule/C_Alchemy.h"

namespace hooks {

// C_Alchemy::End (0x1809F1A8C, REL::ID 54603) -- the session teardown (unregisters the game's own
// six "alchemy" delegates, pops the control token, tears the director down).  Pre-call: remove the
// Auto Brew prompt while the registration object state is still coherent.
class AlchemyEndHook {
public:
    static bool Install();
    static void Uninstall();

private:
    static uint64_t AlchemyEnd(wh::playermodule::C_Alchemy* self);

    inline static REL::Relocation<decltype(&AlchemyEnd)> orig;
};

}  // namespace hooks
