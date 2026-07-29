#pragma once
#include "C_PortRef.h"
#include "C_TemplatedNode.h"

// -----------------------------------------------
// wh::conceptmodule::C_MakeArray -- build an array from input pins
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xC8, vtable 0x183A473B8 (42 slots).
// 5,623 corpus uses.
// -----------------------------------------------
// Ctor 0x1806B31CC, creator 0x1806B39D4; rttr "wh::conceptmodule::MakeArray"
// (reg sub_180C2CD1C).  Editor text: "Creates array from series of input items, for
// reference types only unique elements are stored".  The authored A..X pins are NOT
// C++ members: CreatePorts (0x1806A03D4) adds a runtime port for ANY pin name in
// the XML, and GetPortValue [12] 0x1806A98B8 consumes them POSITIONALLY -- it
// default-constructs a container of TypeT (rttr::type::create), makes a
// variant_sequential_view and appends every port's value from the node's own
// m_ports vector.  The reflected "Value" pin (variadic, TypeT_0) only tells the
// editor to offer more slots.

namespace wh::conceptmodule {

class C_MakeArray : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x1806A98B8 append-all-ports -> container variant
    void EnumerateNodeVariants() override;  // [27] 0x180EF6B58 descriptor "MakeArray", category 0xA

    C_PortRef m_value;   // +0x48  rttr "Value" -- variadic In prototype (Template = TypeT_0)
    C_PortRef m_array;   // +0x88  rttr "Array" -- Out (Template = TypeT)
};
static_assert(sizeof(C_MakeArray) == 0xC8, "C_MakeArray must be 0xC8");

}  // namespace wh::conceptmodule
