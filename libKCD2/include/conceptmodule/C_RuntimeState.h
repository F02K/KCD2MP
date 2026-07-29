#pragma once
#include <cstdint>
#include "conceptmodule/C_Node.h"

// -----------------------------------------------
// wh::conceptmodule::C_RuntimeState -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40 (= C_Node).
// -----------------------------------------------
// RTTI .?AVC_RuntimeState@conceptmodule@wh@@ (TD 0x184AB3370). Concept-graph node base
// (chain C_Node -> C_SharedResource); its node vtable has 42 slots (vs C_Effect's 46).
// [G4c] CHD shows nContained=2 (C_Node, C_SharedResource) and its sole analyzed
// subclass C_CutsceneHandler places its first own member (the I_CutsceneChangeListener
// base) at +0x40 -- so C_RuntimeState ADDS NO DATA over C_Node. Modeled as an empty
// C_Node subclass; the runtime-state node API (42-slot table) is NOT modeled as C++
// virtuals per module convention. UNVERIFIED beyond the CHD walk -- flagged for the
// conceptmodule wave to deep-RE.

namespace wh::conceptmodule {

class C_RuntimeState : public C_Node {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RuntimeState;
};
static_assert(sizeof(C_RuntimeState) == 0x40, "C_RuntimeState adds no data over C_Node (CHD; see header note)");

}  // namespace wh::conceptmodule
