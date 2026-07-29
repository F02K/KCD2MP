#pragma once
#include "C_TriggerPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_InputTriggerPort -- trigger-consuming pin (the sink)
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x30, vtable 0x183A46368 (31 slots).
// -----------------------------------------------
// Ctor/creator 0x1806B2158 (alloc 0x30 @0x1806B21A6).  Stores NO links (trigger
// edges live on the producer).  Direction In, role InputTrigger -- a valid edge
// TARGET only.  Trigger [15] = 0x1806920D4: if IsTriggerSink, build an
// S_NodeExecuteContext{this} on the stack and run the owner node's Execute
// directly (devirtualised 0x180692060) -- no fan-out, no sort.

namespace wh::conceptmodule {

class C_InputTriggerPort : public C_TriggerPort {
public:
    RTTR_ENABLE(C_TriggerPort)   // [5] 0x18061B698, [7] 0x18265355C
    void Trigger() override;     // [15] 0x1806920D4 run the owner node
};
static_assert(sizeof(C_InputTriggerPort) == 0x30, "C_InputTriggerPort adds no data");

}  // namespace wh::conceptmodule
