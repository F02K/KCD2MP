#pragma once
#include "C_TriggerPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_ActiveTriggerPort -- fan-out trigger dispatcher layer
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x30 (adds no data), vtable
// 0x183E255A0 (31 slots).
// -----------------------------------------------
// Base of the producing trigger ports.  Trigger [15] = 0x18061D688, THE dispatcher:
// depth guard (dword_1854807BC vs CVar limit -- overflow logs "Infinite loop
// detected at port:'%s', stopping execution!"), then Collect 0x18061DD3C walks
// downstream (ForEachConnected dir 2) gathering an S_PendingExecute (0x58:
// { S_NodeExecuteContext ctx; std::function fn; int32 order }) for every
// IsTriggerSink port's owner node, sorts ASCENDING by owner GetExecutionOrder
// (comparator 0x18061DC68 -- effects/auto-triggerables run last), then invokes
// each -> C_Node::Execute(node, ctx).  Entirely stack-local: dispatch is IMMEDIATE
// and synchronous, nothing is queued.  rttr creators alloc 0x30 (sub_18264B13C).

namespace wh::conceptmodule {

class C_ActiveTriggerPort : public C_TriggerPort {
public:
    RTTR_ENABLE(C_TriggerPort)   // [5] 0x181A6D980, [7] 0x18265347C
    void Trigger() override;     // [15] 0x18061D688 collect -> sort -> dispatch
};
static_assert(sizeof(C_ActiveTriggerPort) == 0x30, "C_ActiveTriggerPort adds no data");

}  // namespace wh::conceptmodule
