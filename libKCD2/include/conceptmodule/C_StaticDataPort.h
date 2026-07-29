#pragma once
#include "C_InterfaceDataPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_StaticDataPort -- static-instance boundary data pin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  vtable 0x183B895B8.  [sizeof UNVERIFIED,
// >= 0x58; added members untabulated]
// -----------------------------------------------
// RTTI: derives C_InterfaceDataPort.  Ctor 0x18143996C, rttr creator 0x18266CBE0.
// One of the eight port-type rttr names the C_ConceptNodeFactory keeps by string
// (static-instance composite plumbing).
// Layout beyond the C_InterfaceDataPort base was not tabulated; no static_assert.

namespace wh::conceptmodule {

class C_StaticDataPort : public C_InterfaceDataPort {
public:
    RTTR_ENABLE(C_InterfaceDataPort)
};

}  // namespace wh::conceptmodule
