#include "Autobrew.h"

#include "KCSE/KCSEAPI.h"

#include "core/Executor.h"
#include "tasks/Tick/Tick.h"

namespace Autobrew {

void OnPluginLoad()
{
    // The one driver task runs for the plugin's whole lifetime and no-ops while g_executor is
    // null -- a permanent task needs no (re)scheduling bookkeeping and cannot race session
    // teardown/startup the way per-session scheduling could.
    KCSE::GetTaskInterface()->AddTask(&Tick);
}

void OnSessionStart(wh::playermodule::C_Alchemy* session)
{
    g_executor.reset();   // unregister a leftover session's rows BEFORE the new ctor registers
    g_executor = std::make_unique<Executor>(session);
}

void OnSessionEnd(wh::playermodule::C_Alchemy* /*session*/)
{
    g_executor.reset();
}

bool OnBrewResult(wh::playermodule::C_Alchemy* session, char code,
                  wh::entitymodule::C_Item* item, uint32_t count)
{
    return g_executor && g_executor->HandleBrewResult(session, code, item, count);
}

}  // namespace Autobrew
