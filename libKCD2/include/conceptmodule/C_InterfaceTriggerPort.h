#pragma once
#include "C_ActiveTriggerPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_InterfaceTriggerPort -- module-boundary trigger pass-through
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x68, vtable 0x183A46DF8 (31 slots).
// -----------------------------------------------
// Trigger pin on a C_Input/C_Output boundary node: runtime-assigned direction
// (GetDirection [8] = 0x180DBC488 reads m_direction) and links on BOTH sides.  Role
// InterfaceTrigger -> valid edge source AND target; IsPassThrough true.  Ctor
// 0x1826F4266 (alloc 0x68).  [18] ForEachConnected 0x18061E150: dir 1/3 walks
// m_incoming, dir 2/3 walks m_outgoing (side assignment LIKELY -- both are passed
// dir 2 to the worker).  Full edge-link surface [23..28] overridden; fires via the
// inherited dispatcher Trigger [15] 0x18061D688.

namespace wh::conceptmodule {

class C_InterfaceTriggerPort : public C_ActiveTriggerPort {
public:
    RTTR_ENABLE(C_ActiveTriggerPort)   // [5] 0x18061B924, [7] 0x180F8C07C

    int32_t m_direction;               // +0x30  runtime E_PortDirection (0 until assigned)
    uint8_t _pad34[4];                 // +0x34
    std::vector<S_ResourceRef> m_outgoing;  // +0x38  downstream side [LIKELY]
    std::vector<S_ResourceRef> m_incoming;  // +0x50  upstream side [LIKELY]
};
static_assert(sizeof(C_InterfaceTriggerPort) == 0x68, "C_InterfaceTriggerPort must be 0x68");

}  // namespace wh::conceptmodule
