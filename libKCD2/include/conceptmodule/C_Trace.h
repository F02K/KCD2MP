#pragma once
#include "C_PortRef.h"
#include "C_TemplatedNode.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"

// -----------------------------------------------
// wh::conceptmodule::C_Trace -- log-a-value debug node
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x148, vtable 0x183BCFA88
// (42 slots).  174 corpus uses.
// -----------------------------------------------
// Ctor 0x1815F63D0, creator 0x1815F6320; rttr "wh::conceptmodule::Trace"
// (reg sub_181477E24).  On Exec (OnExecute [33] 0x18268E0B4) logs the generic
// Value pin at the severity of the Level pin, then fires the out trigger.

namespace wh::conceptmodule {

enum class E_TraceLevel : int32_t;   // registered trace-severity enum [values unread]

class C_Trace : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides
    void EnumerateNodeVariants() override;  // [27] 0x18268EB98
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x18268E0B4

    C_TypedPortRef<S_Trigger>    m_exec;        // +0x48  "Exec"  In trigger
    C_PortRef                    m_value;       // +0x88  "Value" In (Template = TypeT)
    C_TypedPortRef<E_TraceLevel> m_level;       // +0xC8  "Level" In [offset LIKELY]
    C_TypedPortRef<S_Trigger>    m_outTrigger;  // +0x108 Out trigger [name UNVERIFIED -- "OnExec"?]
};
static_assert(sizeof(C_Trace) == 0x148, "C_Trace must be 0x148");

}  // namespace wh::conceptmodule
