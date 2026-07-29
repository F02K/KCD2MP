#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::E_PortDirection -- which way a port faces
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).
// -----------------------------------------------
// Registered rttr enum (name string 0x183E39B90, reg sub_1800A4D10) with enumerators
// "In", "Out", "Bi".  Numeric values PROVEN from the constant-return
// I_Port::GetDirection overrides (0x181A72470 -> 1 on Input* ports, 0x181A72480 ->
// 2 on Output*/Constant/Asset ports) and the GetOrCreatePort switch (0x180699630:
// 1/2/3 -> Input/Output/Interface port class); In/Out/Bi <-> 1/2/3 pairing is the
// natural reading of both [enumerator-value pairing LIKELY].  Bi = module-boundary
// interface pin (In from outside the module, Out from inside); those ports store
// their effective direction at runtime (+0x30).

namespace wh::conceptmodule {

enum class E_PortDirection : int32_t {
    None = 0,   // unresolved / definition missing (not a registered enumerator)
    In   = 1,
    Out  = 2,
    Bi   = 3,   // interface (module-boundary) pin
};

}  // namespace wh::conceptmodule
