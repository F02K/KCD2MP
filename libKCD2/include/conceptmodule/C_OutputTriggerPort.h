#pragma once
#include "C_ActiveTriggerPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_OutputTriggerPort -- trigger-producing pin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x48, vtable 0x183A46B98 (31 slots).
// -----------------------------------------------
// Ctor/creator 0x1806B2244 (alloc 0x48 @0x1806B2292).  Owns the downstream links
// (trigger edges live on the producer).  Direction Out, role OutputTrigger -- a
// valid edge SOURCE only.  Overrides: [18] ForEachConnected 0x18061E068 (always
// m_outgoing, dir 2), [23] HasOutgoing 0x180693BDC (linear find), [25] AddOutgoing
// 0x180693758 (push handle), [27] ClearOutgoing 0x180D2ACF0, [12] 0x1806B3C98 [U],
// [29] 0x181110E6C / [30] 0x182650654 [U].  Fired via the inherited dispatcher
// Trigger [15] 0x18061D688 (e.g. C_If's True/False pins).

namespace wh::conceptmodule {

class C_OutputTriggerPort : public C_ActiveTriggerPort {
public:
    RTTR_ENABLE(C_ActiveTriggerPort)   // [5] 0x180FA57E0, [7] 0x18265359C

    std::vector<S_ResourceRef> m_outgoing;   // +0x30  downstream sink links
};
static_assert(sizeof(C_OutputTriggerPort) == 0x48, "C_OutputTriggerPort must be 0x48");

}  // namespace wh::conceptmodule
