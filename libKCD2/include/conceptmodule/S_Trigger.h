#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::S_Trigger -- KCD2 WHGame.dll 1.5.6 (kd7u).  Trigger tag type.
// -----------------------------------------------
// The concept-graph "trigger pulse" port payload: S_Trigger-typed ports
// (C_TypedPortRef<S_Trigger> / trigger OUTPUT ports fired via port vf[+0x78]) carry no
// data -- the type exists to tag the port as an edge-triggered signal. Referenced by
// the guimodule trigger nodes (C_ApseViewTrigger on-enter/on-exit, C_PlayTrackView,
// C_InteractorOverride, C_CutsceneHandler outputs) and dialog/fader nodes.
// Internal representation UNVERIFIED (never stored by value in any analyzed host --
// C_TypedPortRef does not embed T); modeled as an empty tag struct.

namespace wh::conceptmodule {

struct S_Trigger {};

}  // namespace wh::conceptmodule
