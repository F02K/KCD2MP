#pragma once

#include <cstdint>

namespace wh::playermodule { class C_Alchemy; }
namespace wh::entitymodule { class C_Item; }

// Autobrew plugin surface.  All driver state lives in one Executor instance (core/Executor.h)
// whose lifetime IS the brewing session; these free functions are the plugin/hook seams.
namespace Autobrew {

// Called once from KCSE_PLUGIN_LOAD: starts the one permanent per-frame task (tasks/Tick/,
// the driver state machine) that no-ops while no session is live.
void OnPluginLoad();

// C_Alchemy::Start / End hooks.
void OnSessionStart(wh::playermodule::C_Alchemy* session);
void OnSessionEnd(wh::playermodule::C_Alchemy* session);

// Gated brew-result presenter hook (hooks/BrewResult/): true = batch verdict toasted, skip the
// m_onBrewResult emit (suppresses the game-pausing reward modal); false = run vanilla.
bool OnBrewResult(wh::playermodule::C_Alchemy* session, char code,
                  wh::entitymodule::C_Item* item, uint32_t count);

}  // namespace Autobrew
