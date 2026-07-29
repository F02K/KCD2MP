#pragma once
#include "C_FunctionBase.h"

// -----------------------------------------------
// wh::conceptmodule::C_Function -- free-function call node
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x68 (adds no data), vtable
// 0x183E2EA00 (42 slots).  24,906 corpus uses -- the workhorse of Skald.
// -----------------------------------------------
// Creator 0x1806B1F24.  DeclaringType stays invalid -> ResolveMethod takes the
// GLOBAL method-registry branch.  PULL-evaluated: GetPortValue [12] 0x18069133C
// re-INVOKES the method on every read of the return pin (no caching -- side effects
// fire per read; contrast C_EventFunction).  Void methods instead get Exec/OnExec
// triggers and run through the inherited OnExecute [33].  EnumerateNodeVariants
// [27] 0x18068FCA4 materializes one palette variant per reflected global method
// with metadata[0] == 1.

namespace wh::conceptmodule {

class C_Function : public C_FunctionBase {
public:
    RTTR_ENABLE(C_FunctionBase)    // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x18069133C invoke -> result
    void EnumerateNodeVariants() override;  // [27] 0x18068FCA4 one variant per exposed global method
};
static_assert(sizeof(C_Function) == 0x68, "C_Function adds no data");

}  // namespace wh::conceptmodule
