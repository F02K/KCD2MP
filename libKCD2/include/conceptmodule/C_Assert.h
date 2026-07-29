#pragma once
#include "C_PortRef.h"
#include "C_TemplatedNode.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"

// -----------------------------------------------
// wh::conceptmodule::C_Assert -- value-equality assertion (debug-only)
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x148, vtable 0x1846ECE00
// (42 slots).  Absent from the shipping graph corpus.
// -----------------------------------------------
// Ctor 0x18268D780, creator 0x182685490; rttr "wh::conceptmodule::Assert"
// (reg sub_180C2E2A0).  On Exec (OnExecute [33] 0x18268E058) compares Value against
// Equals; note the port order differs from C_Trace: the out trigger sits SECOND
// (+0x88), before the two data pins.

namespace wh::conceptmodule {

class C_Assert : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides
    void EnumerateNodeVariants() override;  // [27] 0x18268E334
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x18268E058

    C_TypedPortRef<S_Trigger> m_exec;        // +0x48  "Exec"   In trigger
    C_TypedPortRef<S_Trigger> m_outTrigger;  // +0x88  Out trigger [name UNVERIFIED]
    C_PortRef                 m_value;       // +0xC8  "Value"  In (Template = TypeT)
    C_PortRef                 m_equals;      // +0x108 "Equals" In (expected value)
};
static_assert(sizeof(C_Assert) == 0x148, "C_Assert must be 0x148");

}  // namespace wh::conceptmodule
