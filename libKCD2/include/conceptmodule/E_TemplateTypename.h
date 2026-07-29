#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::E_TemplateTypename -- which generic slot a pin's type binds to
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).
// -----------------------------------------------
// Registered rttr enum "wh::conceptmodule::TemplateTypename" (name string 0x183E39C40,
// reg sub_1800A6170).  Values PROVEN by C_TemplatedNode::GetTemplateType 0x1806A8E10:
// 1 -> m_typeT itself; 2/3 -> the 1st/2nd template ARGUMENT of m_typeT (e.g. for
// TypeT = wh::conceptmodule::Floats = std::vector<float>, TypeT_0 = float) -- one XML
// attribute types both the container pin and the element pin.  Surfaced on
// definition::I_PortDefinition as the "Template" property (wrapper vt 0x183A88460).

namespace wh::conceptmodule {

enum class E_TemplateTypename : int32_t {
    None    = 0,
    TypeT   = 1,
    TypeT_0 = 2,   // first template argument of TypeT
    TypeT_1 = 3,   // second template argument of TypeT (no consumer observed)
};

}  // namespace wh::conceptmodule
