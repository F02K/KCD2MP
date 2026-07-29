#pragma once
#include "C_DataPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_InputDataPort -- data-consuming pin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x38, vtable 0x183A46730 (32 slots).
// -----------------------------------------------
// Ctor/creator 0x1806B2794 (alloc 0x38 @0x1806B27E2).  Stores its single upstream
// link on itself (data edges live on the consumer).  Direction In (0x181A72470),
// role InputData -- a valid edge TARGET only.  Overrides: [12] 0x1806B3D98 [U],
// [18] ForEachConnected 0x1806937AC (one-element range over m_incoming, dir
// forwarded), [24] HasIncoming 0x180D2AF4C (resolve+compare), [26] AddIncoming
// 0x180D2AF28 (overwrite m_incoming), [28] ClearIncoming 0x180D2B05C ({-1,0}),
// [29] 0x181097E84 [U], [31] GetValueImpl inherits the pull-with-upstream-fallback.

namespace wh::conceptmodule {

class C_InputDataPort : public C_DataPort {
public:
    RTTR_ENABLE(C_DataPort)   // [5] 0x1806B1BB0, [7] 0x18265353C

    S_ResourceRef m_incoming;   // +0x30  the one upstream producer (ctor {-1, 0})
};
static_assert(sizeof(C_InputDataPort) == 0x38, "C_InputDataPort must be 0x38");

}  // namespace wh::conceptmodule
