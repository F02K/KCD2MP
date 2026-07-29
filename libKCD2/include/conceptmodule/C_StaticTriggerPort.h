#pragma once
#include "C_InterfaceTriggerPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_StaticTriggerPort -- static-instance boundary trigger pin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  vtable 0x183B819B0.  [sizeof UNVERIFIED,
// >= 0x68; added members untabulated]
// -----------------------------------------------
// RTTI: derives C_InterfaceTriggerPort.  Ctor 0x181412D74, rttr creator 0x18266CC8C.
// One of the eight port-type rttr names the C_ConceptNodeFactory keeps by string
// (static-instance composite plumbing).  Layout beyond the base was not tabulated;
// no static_assert.

namespace wh::conceptmodule {

class C_StaticTriggerPort : public C_InterfaceTriggerPort {
public:
    RTTR_ENABLE(C_InterfaceTriggerPort)
};

}  // namespace wh::conceptmodule
