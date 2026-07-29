#pragma once

// -----------------------------------------------
// wh::entitymodule::I_WorldInventoryListener -- world-inventory change listener
// (KCD2 WHGame.dll 1.5.6, kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_WorldInventoryListener@entitymodule@wh@@.  No COL of its own (never
// most-derived).  Registered in C_WorldInventory's
// wh::shared::C_Listeners<I_WorldInventoryListener,4> @+0x178 (see
// entitymodule/C_WorldInventory.h).  Slot count (3) read from the sole enumerated
// implementor vtable, xgenaimodule::C_PerceptionItemListener (0x183FE6C40):
// [0] sub_1819DBEF4, [1] sub_181850510, [2] sub_1832AE9A4.  Slot roles UNRESOLVED;
// whether [0] is a virtual-dtor slot is UNVERIFIED.  Trailing slots beyond [2]
// cannot be excluded from a single implementor.

namespace wh::entitymodule {

class I_WorldInventoryListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_WorldInventoryListener;
    virtual void _vf0() = 0;   // [0] [U role; dtor placement unverified]
    virtual void _vf1() = 0;   // [1] [U role]
    virtual void _vf2() = 0;   // [2] [U role]
};
static_assert(sizeof(I_WorldInventoryListener) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::entitymodule
