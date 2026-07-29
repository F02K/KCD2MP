#pragma once

// -----------------------------------------------
// wh::entitymodule::I_InventoryManagerListener -- inventory-manager event
// listener interface (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_InventoryManagerListener@entitymodule@wh@@.  Shape
// (7 slots) read from the sole enumerated implementor
// xgenaimodule::C_AIInventoryManager (vtable rva 0x3FECD38: [0] dtor
// sub_180A308D4, [1]sub_180A2F418 [2]sub_180966824 [3]sub_180965C4C
// [4]sub_1832F00E4 [5]sub_1812EBD54 [6]sub_1832EDFF8).  Listeners self-append
// into the inventory subsystem's listener vector (+1049176/+1049184).
// OnItemAdded/Removed-style roles NOT confirmed [U].

namespace wh::entitymodule {

class I_InventoryManagerListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_InventoryManagerListener;
    virtual ~I_InventoryManagerListener();  // [0]
    virtual void ImlVf1() = 0;   // [1] [U role]
    virtual void ImlVf2() = 0;   // [2] [U role]
    virtual void ImlVf3() = 0;   // [3] [U role]
    virtual void ImlVf4() = 0;   // [4] [U role]
    virtual void ImlVf5() = 0;   // [5] [U role]
    virtual void ImlVf6() = 0;   // [6] [U role]
};
static_assert(sizeof(I_InventoryManagerListener) == 0x08, "interface: vtable pointer only");

}  // namespace wh::entitymodule
