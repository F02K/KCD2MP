#pragma once
#include "C_EdgePort.h"

// -----------------------------------------------
// wh::conceptmodule::C_TriggerPort -- pulse-carrying port layer
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x30 (adds no data, no new slots),
// vtable 0x183A46468 (31 slots).
// -----------------------------------------------
// Sibling of C_DataPort under C_EdgePort; carries no value, only Trigger() pulses.
// Trigger edges are stored on the PRODUCER side (C_OutputTriggerPort /
// C_InterfaceTriggerPort vectors); dispatch is immediate and synchronous.
// rttr constructor_wrapper vftable 0x183C30840 (alloc 0x30).

namespace wh::conceptmodule {

class C_TriggerPort : public C_EdgePort {
public:
    RTTR_ENABLE(C_EdgePort)   // [5] 0x181A6CB2C, [7] 0x1826535BC
};
static_assert(sizeof(C_TriggerPort) == 0x30, "C_TriggerPort adds no data");

}  // namespace wh::conceptmodule
