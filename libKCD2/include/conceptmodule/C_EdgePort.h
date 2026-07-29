#pragma once
#include <cstdint>
#include "I_Port.h"

// -----------------------------------------------
// wh::conceptmodule::C_EdgePort -- wireable graph-port layer
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x30 (adds no data), vtable
// 0x183A46268 (31 slots).
// -----------------------------------------------
// Splits into C_DataPort (adds typed GetValueImpl, 32 slots) and C_TriggerPort
// (31 slots).  Adds the edge-link surface [23..30]; storage lives in the leaves:
// data edges are stored on the CONSUMER (input side, single S_ResourceRef), trigger
// edges on the PRODUCER (output side, vector<S_ResourceRef>).  Wiring writer:
// Connect 0x1806986AC -- AddOutgoing on the source iff CanBeEdgeSource (role in
// {InterfaceData, OutputTrigger, InterfaceTrigger}, 0x1806943A4), AddIncoming on the
// target iff CanBeEdgeTarget (role in {InputData, InterfaceData, InterfaceTrigger},
// 0x18069437C).  rttr creators alloc 0x30 (e.g. sub_18264B538).  Overrides
// IsPassThrough to `role == InterfaceData`.

namespace wh::conceptmodule {

class C_EdgePort : public I_Port {
public:
    RTTR_ENABLE(I_Port)                                 // [5..7] trio overrides
    virtual bool HasOutgoing(_smart_ptr<I_Port> port);  // [23] base 0x1826506C4 "not supported" (release arg, false)
    virtual bool HasIncoming(_smart_ptr<I_Port> port);  // [24] base "not supported"
    virtual void AddOutgoing(_smart_ptr<I_Port> port);  // [25] base "not supported"; called by Connect @0x1806986E1
    virtual void AddIncoming(_smart_ptr<I_Port> port);  // [26] base "not supported"; called by Connect @0x18069870F
    virtual void ClearOutgoing();                       // [27] base "not supported" [sig U]
    virtual void ClearIncoming();                       // [28] base "not supported" [sig U]
    virtual void unk29();                               // [29] base nullsub; overridden by every connectable leaf [U -- serialization candidate]
    virtual void unk30();                               // [30] base nullsub; overridden by Interface*/OutputTrigger leaves [U]
};
static_assert(sizeof(C_EdgePort) == 0x30, "C_EdgePort adds no data");

}  // namespace wh::conceptmodule
