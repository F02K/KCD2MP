#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_DamageTypeMask -- stab/slash/smash component bitmask (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Name RE-CHOSEN (no RTTI/string source). sub_182CF6204(float[3]) builds it: bit0 if [0]!=0,
// bit1 if [1]!=0, bit2 if [2]!=0 -- exactly the 3 components of StabSlashSmashValue
// (entitymodule/StabSlashSmashValue.h). sub_180C64938/sub_180C65B38 iterate the same 3 bits
// to find the max of a bitmask-selected component triple.

namespace wh::rpgmodule {

enum class E_DamageTypeMask : uint32_t {
    None  = 0,
    Stab  = 1u << 0,
    Slash = 1u << 1,
    Smash = 1u << 2,
    All   = Stab | Slash | Smash,
};

}  // namespace wh::rpgmodule
