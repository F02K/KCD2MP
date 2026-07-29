#pragma once
#include "C_FunctionBase.h"

// -----------------------------------------------
// wh::conceptmodule::C_MemberFunction -- instance-method call node
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x68 (adds no data), vtable
// 0x183E2E750 (42 slots).  2,314 corpus uses.
// -----------------------------------------------
// Creator 0x180FC53A8.  DeclaringType is set (XML Constant) -> ResolveMethod uses
// type.get_method(MethodName).  The instance arrives through an In data pin typed
// by the declaring type -- named by metadata key 4, default "Target":
// GetPortDefinitions [28] 0x180691550 emits it first, then the inherited
// parameter/return/trigger pins.  GetPortValue [12] 0x18061AB8C reads the Target
// pin, converts the variant to an instance descriptor, then invokes.

namespace wh::conceptmodule {

class C_MemberFunction : public C_FunctionBase {
public:
    RTTR_ENABLE(C_FunctionBase)    // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x18061AB8C Target -> instance -> invoke
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x180691550 Target pin + base pins
};
static_assert(sizeof(C_MemberFunction) == 0x68, "C_MemberFunction adds no data");

}  // namespace wh::conceptmodule
