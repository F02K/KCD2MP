#pragma once
#include "C_InterfaceDataPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_ConstantInterfacePort -- literal on a module-boundary pin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x70, vtable 0x183A45BA8 (32 slots).
// -----------------------------------------------
// CreatePorts (0x1806A03D4) picks this over C_ConstantPort when the definition's
// IsInterface predicate (def vslot 17) is true.  Named ctor sub_182732ABC(name,
// std::string(value)) stores the raw literal; default ctor 0x1806B1748 leaves the
// variant INVALID (policy 0x1804FBC18 at +0x68) -- which is exactly what makes
// GetValueImpl fall through to the edge traversal.  GetValue [16] = 0x181765A2C;
// IsPassThrough [10] = 0x1806B3C0C.

namespace wh::conceptmodule {

class C_ConstantInterfacePort : public C_InterfaceDataPort {
public:
    RTTR_ENABLE(C_InterfaceDataPort)   // [5] 0x18061B940, [7] 0x180FE93BC
    rttr::variant GetValue() override; // [16] 0x181765A2C

    rttr::variant m_value;   // +0x58  the literal (std::string policy; invalid when unset)
};
static_assert(sizeof(C_ConstantInterfacePort) == 0x70, "C_ConstantInterfacePort must be 0x70");

}  // namespace wh::conceptmodule
