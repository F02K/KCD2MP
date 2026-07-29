#pragma once
#include "C_PortRef.h"
#include "C_TemplatedNode.h"
#include "rttr/type.h"

// -----------------------------------------------
// wh::conceptmodule::C_Switch -- pull-evaluated value multiplexer
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xD0, vtable 0x183AF87D0 (42 slots).
// 1,682 corpus uses.
// -----------------------------------------------
// Ctor 0x18105DED0, creator 0x18105DE18; rttr type "wh::conceptmodule::Switch"
// (reg sub_180F83F90).  NO OnExecute override -- pure data-flow router evaluated on
// pull.  Pin grammar (GetPortDefinitions [28] 0x1804F22BC): In = "<Case>Value<N>"
// per SwitchValues case + "DefaultValue<N>", Out = "Value<N>", N = 1..len(ValueTypes)
// (1-BASED).  GetPortValue [12] 0x18061ECC0: convert the Switch pin's value to
// string; if not in m_switchValues use "Default"; forward input pin
// "<Case>Value<N>" for the requested output "Value<N>" (miss -> "switch clause '%s'
// does not match any specific input port and default input port is undefined...").
// Switch is a RAW C_PortRef because the discriminator type is generic (TypeT).

namespace wh::conceptmodule {

class C_Switch : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x18061ECC0 case-select + forward
    void EnumerateNodeVariants() override;  // [27]
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x1804F22BC case groups (emitter sub_1804F2EC8)

    C_PortRef m_switch;             // +0x48  rttr "Switch" -- the discriminator pin (untyped/TypeT)
    std::vector<uint64_t> m_unk88;  // +0x88  ctor-zeroed, never touched in analyzed paths [element type U]
    std::vector<CryStringT<char>> m_switchValues;  // +0xA0  rttr "SwitchValues" -- the case names
    std::vector<rttr::type> m_valueTypes;          // +0xB8  rttr "ValueTypes" -- one routed value slot per entry
};
static_assert(sizeof(C_Switch) == 0xD0, "C_Switch must be 0xD0");

}  // namespace wh::conceptmodule
