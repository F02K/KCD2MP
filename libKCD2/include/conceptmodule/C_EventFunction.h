#pragma once
#include "C_EventNode.h"
#include "C_Function.h"

// -----------------------------------------------
// wh::conceptmodule::C_EventFunction -- push-evaluated free-function call
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x90, vtable 0x183E2EE18 (43 slots).
// 824 corpus uses.
// -----------------------------------------------
// Chain: C_FunctionBase -> C_Function -> C_EventNode<C_Function> (vt 0x183E2ECB8)
// -> here.  Creator 0x1811A3B04.  The push-mode twin of C_Function: OnExecute [33]
// 0x180618A94 invokes the method exactly ONCE per Exec, snapshots the result into
// the event-data map keyed by the return-pin name, then EmitEvents OnExec [42] --
// outputs are readable only while OnExec propagates.  GetPortValue [12] 0x18127FAA4
// does NOT invoke (event-map lookup only).  GetPortDefinitions [28] 0x18101E2C0 =
// base pins + Exec/OnExec unconditionally.

namespace wh::conceptmodule {

class C_EventFunction : public C_EventNode<C_Function> {
public:
    RTTR_ENABLE(C_EventNode<C_Function>)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x18127FAA4 event-map lookup
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x18101E2C0 base + Exec/OnExec
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x180618A94 invoke once -> snapshot -> EmitEvent(OnExec)
};
static_assert(sizeof(C_EventFunction) == 0x90, "C_EventFunction adds no data over C_EventNode<C_Function>");

}  // namespace wh::conceptmodule
