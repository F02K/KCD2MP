#pragma once
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::rpgmodule::I_SoulResolver -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (vtable only).
// -----------------------------------------------
// Primary base of C_SoulList (shares its 39-slot combined primary vtable 0x183F44DD8). Slot [0]
// is the public WUID->soul lookup (C_SoulList impl 0x1823C9C34 -> generic slot-map lookup
// sub_181F985D0(this+0x38, wuid); soul WUID tag = 5). The remaining combined slots were NOT
// enumerated and this interface's own slot extent is UNVERIFIED -- only [0] is declared.
// Namespace inferred from the implementor (no direct RTTI quote), flagged.

namespace wh::rpgmodule {

class C_Soul;

class I_SoulResolver {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SoulResolver;
    virtual C_Soul* GetSoulByWuid(wh::framework::WUID a_wuid) = 0;   // [0]
};

}  // namespace wh::rpgmodule
