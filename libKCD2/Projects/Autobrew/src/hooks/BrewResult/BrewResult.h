#pragma once

#include <cstdint>

#include "REL.h"
#include "playermodule/C_Alchemy.h"

namespace wh::entitymodule { class C_Item; }

namespace hooks {

// The gated brew-result presenter (0x182E18ED0, REL::ID 375581): the `if (m_presentResult)
// emit m_onBrewResult(code, potion, count)` helper whose ONLY callers are GrantBrewReward's two
// verdict sites -- so every call here IS a potion result for `self`, no judging needed.  Batch
// verdicts with an item become Autobrew's center toast (skipping the emit = no pausing reward
// modal); everything else runs the original.  GradeBrew's no-recipe fail emits directly
// (0x182E166FB), deliberately unhooked: fail messages stay fully vanilla.
class BrewResultHook {
public:
    static bool Install();
    static void Uninstall();

private:
    static void PresentResult(wh::playermodule::C_Alchemy* self, char code,
                              wh::entitymodule::C_Item* item, uint32_t count);

    inline static REL::Relocation<decltype(&PresentResult)> orig;
};

}  // namespace hooks
