#pragma once
#include "C_EventNode.h"
#include "C_MemberFunction.h"

// -----------------------------------------------
// wh::conceptmodule::C_EventMemberFunction -- push-evaluated instance-method call
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x90, vtable 0x183E2EB58 (43 slots).
// 289 corpus uses.
// -----------------------------------------------
// Chain: C_FunctionBase -> C_MemberFunction -> C_EventNode<C_MemberFunction>
// (vt 0x183E2EF78) -> here.  Creator 0x181187338.  Same push semantics as
// C_EventFunction but the invoke path reads the Target pin first
// (C_MemberFunction::GetPortValue).  OnExecute [33] 0x180619EA8; GetPortValue [12]
// 0x1826B9CA8 (event-map lookup); GetPortDefinitions [28] 0x180F816AC (Target +
// base pins + Exec/OnExec); EmitEvent [42] shared 0x180618A5C.

namespace wh::conceptmodule {

class C_EventMemberFunction : public C_EventNode<C_MemberFunction> {
public:
    RTTR_ENABLE(C_EventNode<C_MemberFunction>)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x1826B9CA8 event-map lookup
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x180F816AC
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x180619EA8 Target -> invoke once -> snapshot -> EmitEvent(OnExec)
};
static_assert(sizeof(C_EventMemberFunction) == 0x90, "C_EventMemberFunction adds no data over C_EventNode<C_MemberFunction>");

}  // namespace wh::conceptmodule
