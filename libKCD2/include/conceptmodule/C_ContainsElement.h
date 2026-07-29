#pragma once
#include "C_PortRef.h"
#include "C_TemplatedNode.h"
#include "C_TypedPortRef.h"

// -----------------------------------------------
// wh::conceptmodule::C_ContainsElement -- array membership test
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x108, vtable 0x183B2EF78
// (42 slots).  215 corpus uses.
// -----------------------------------------------
// Ctor 0x1811D8424, creator 0x1811D836C; rttr "wh::conceptmodule::ContainsElement"
// (reg sub_180C2DB14).  GetPortValue [12] 0x180825A98: read Array; if sequential
// container, convert Value to the element type and linear-scan with variant
// equality (sub_1804F86E4) -> bool.  A missing/non-container Array yields FALSE,
// not an error.

namespace wh::conceptmodule {

class C_ContainsElement : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x180825A98
    void EnumerateNodeVariants() override;  // [27] 0x180EF6A58

    C_PortRef m_arrayPort;           // +0x48  rttr "Array" -- In (Template = TypeT)
    C_PortRef m_valuePort;           // +0x88  rttr "Value" -- In needle (Template = TypeT_0)
    C_TypedPortRef<bool> m_result;   // +0xC8  rttr "Result" -- Out
};
static_assert(sizeof(C_ContainsElement) == 0x108, "C_ContainsElement must be 0x108");

}  // namespace wh::conceptmodule
