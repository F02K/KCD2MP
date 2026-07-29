#pragma once
#include "C_PortRef.h"
#include "C_TemplatedNode.h"
#include "C_TypedPortRef.h"

// -----------------------------------------------
// wh::conceptmodule::C_Length -- array length
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xC8, vtable 0x183B07220 (42 slots).
// 134 corpus uses.
// -----------------------------------------------
// Ctor 0x1810D67BC, creator 0x1810D670C; rttr "wh::conceptmodule::Length"
// (reg sub_18109FC74).  GetPortValue [12] 0x1814033B8: read Array; sequential
// container -> int variant of view.get_size() (policy 0x1803E17FC), else empty.

namespace wh::conceptmodule {

class C_Length : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x1814033B8
    void EnumerateNodeVariants() override;  // [27] 0x180EF6758

    C_PortRef m_arrayPort;          // +0x48  rttr "Array" -- In (Template = TypeT)
    C_TypedPortRef<int> m_length;   // +0x88  rttr "Length" -- Out
};
static_assert(sizeof(C_Length) == 0xC8, "C_Length must be 0xC8");

}  // namespace wh::conceptmodule
