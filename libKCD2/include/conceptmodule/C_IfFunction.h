#pragma once
#include "C_FunctionBase.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"

// -----------------------------------------------
// wh::conceptmodule::C_IfFunction -- branch on a reflected call's result
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x128, vtable 0x183A47048
// (42 slots).  5,405 corpus uses.
// -----------------------------------------------
// Ctor 0x1806B2ED4, creator 0x1806B3460; rttr type "wh::conceptmodule::IfFunction"
// (reg sub_18162D254 -- pin offsets baked: Exec 0x68, True 0xA8, False 0xE8).
// OnExecute [33] 0x18061ACD4: call C_FunctionBase::OnExecute (which invokes AND
// fires OnExec), then invoke AGAIN, coerce the result to bool, fire True/False.
// CAUTION for hookers: the observed double-invoke means side-effectful methods may
// run twice per Exec [intent UNVERIFIED].  GetPortDefinitions [28] 0x180FDB8A0
// wraps the base sink in an adapter -- the return pin is consumed as the condition
// rather than exposed [LIKELY].  Argument pins are the inherited per-parameter
// dynamic pins.

namespace wh::conceptmodule {

class C_IfFunction : public C_FunctionBase {
public:
    RTTR_ENABLE(C_FunctionBase)   // [5..7] trio overrides
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x180FDB8A0 filtered base set
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x18061ACD4 invoke -> bool -> True/False

    C_TypedPortRef<S_Trigger> m_exec;   // +0x68  "Exec"  In trigger
    C_TypedPortRef<S_Trigger> m_true;   // +0xA8  "True"  Out trigger
    C_TypedPortRef<S_Trigger> m_false;  // +0xE8  "False" Out trigger
};
static_assert(sizeof(C_IfFunction) == 0x128, "C_IfFunction must be 0x128");

}  // namespace wh::conceptmodule
