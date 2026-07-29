#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_AlchemyPotBoilingState -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper.
// -----------------------------------------------
// RTTR-registered enum (reg sub_18021D230; name string @0x183C0A8F0).  The cauldron's boiling
// state, computed each frame by the pot-effect update sub_180738CBC from the fire intensity
// (C_Alchemy+0x320) vs the boil point 1.0: below -> NoBoiling (heating up), held AT it ->
// WeakBoiling (sustained while the pot stays on the fire), above it (bellows set 2.0, draining
// back) -> StrongBoiling.  WeakBoilTimeout/StrongBoilTimeout (table-entity constants) scale the
// ramp/drain RATES (x0.6 s).  Gate: accrual only while pot-on-fire && !moving (sub_1807391C4).
// Stored as a BYTE at C_Alchemy+0x440, consumed by the boil-time accrual sub_18073920C
// (Weak -> resource+0x14 += dt, Strong -> resource+0x18 += dt).

namespace wh::playermodule {

struct E_AlchemyPotBoilingState {
    enum Type : int32_t {
        NoBoiling     = 0,
        WeakBoiling   = 1,
        StrongBoiling = 2,
    };
};

}  // namespace wh::playermodule
