#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::E_PortSpecification -- the concrete port-class discriminator
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).
// -----------------------------------------------
// Registered rttr enum (name string 0x183E39B68, reg sub_1800A5280) whose
// enumerators are literally the six concrete port-class names.  Numeric values
// PROVEN by decompiling every I_Port vslot-17 override (each a `mov al, N; ret`);
// name<->value pairing follows the class of each override [LIKELY].  Edge rules
// (Connect 0x1806986AC): CanBeEdgeSource (0x1806943A4) -> spec in {InterfaceDataPort,
// OutputTriggerPort, InterfaceTriggerPort}; CanBeEdgeTarget (0x18069437C) -> spec in
// {InputDataPort, InterfaceDataPort, InterfaceTriggerPort}.  Spec None (abstract
// layers + C_PortRef + C_DebuggerPort) can never be an edge endpoint.

namespace wh::conceptmodule {

enum class E_PortSpecification : int32_t {
    None                 = 0,   // abstract layers / C_PortRef / C_DebuggerPort (not a registered enumerator)
    InputDataPort        = 1,
    OutputDataPort       = 2,   // + C_ConstantPort, C_AssetPort
    InterfaceDataPort    = 3,   // + C_ConstantInterfacePort, C_AssetInterfacePort
    InputTriggerPort     = 4,   // + C_AutoTriggerPort
    OutputTriggerPort    = 5,
    InterfaceTriggerPort = 6,
};

}  // namespace wh::conceptmodule
