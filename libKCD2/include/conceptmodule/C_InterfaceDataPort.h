#pragma once
#include "C_DataPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_InterfaceDataPort -- module-boundary data pass-through
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x58, vtable 0x183A46988 (32 slots).
// -----------------------------------------------
// The pin on a C_Input/C_Output boundary node: stores BOTH one upstream link and a
// downstream fan-out, and its direction is RUNTIME-assigned (m_direction, read by
// GetDirection [8] = 0x180DBC488) because a boundary port is In from outside the
// module and Out from inside.  Role InterfaceData -> both a valid edge source AND
// target; IsPassThrough true (skipped by visitor flag bit0).  Ctors 0x1806B2708 /
// 0x18132CD60.  Overrides the full edge-link surface [23..28] plus
// [18] ForEachConnected 0x180693668 (dir 1/3 -> m_incoming; dir 2/3 -> m_outgoing).

namespace wh::conceptmodule {

class C_InterfaceDataPort : public C_DataPort {
public:
    RTTR_ENABLE(C_DataPort)   // [5] 0x18061B908, [7] 0x18069B898

    int32_t m_direction;      // +0x30  runtime E_PortDirection (0 until assigned)
    S_ResourceRef m_incoming; // +0x34  upstream link (ctor {-1, 0})
    uint8_t _pad3C[4];        // +0x3C
    std::vector<S_ResourceRef> m_outgoing;  // +0x40  downstream fan-out
};
static_assert(sizeof(C_InterfaceDataPort) == 0x58, "C_InterfaceDataPort must be 0x58");

}  // namespace wh::conceptmodule
