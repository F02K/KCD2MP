#pragma once

namespace Autobrew {

// The ONE permanent per-frame task (scheduled once by OnPluginLoad, reschedules itself
// forever): routes to Executor::Tick -- the whole driver state machine -- on the live
// session, no-ops between sessions.
void Tick();

}  // namespace Autobrew
