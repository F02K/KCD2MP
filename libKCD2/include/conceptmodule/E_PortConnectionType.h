#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::E_PortConnectionType -- how a pin gets its value
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).
// -----------------------------------------------
// Registered rttr enum (name string 0x183E39C68, reg sub_1800A5B00) with exactly two
// enumerators: "Constant" (literal <Constant Value=.../> in the XML) and "Edge"
// (wired from another port).  Surfaced on definition::I_PortDefinition as the
// "ConnectionType" property (wrapper vt 0x183A882C8) -- authoring-side metadata that
// constrains how the deserializer may feed the pin.  Numeric values UNVERIFIED
// (0/1 in registration order assumed).

namespace wh::conceptmodule {

enum class E_PortConnectionType : int32_t {
    Constant = 0,   // [value UNVERIFIED]
    Edge     = 1,   // [value UNVERIFIED]
};

}  // namespace wh::conceptmodule
