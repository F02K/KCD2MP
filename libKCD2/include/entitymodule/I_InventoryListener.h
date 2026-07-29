#pragma once

// -----------------------------------------------
// wh::entitymodule::I_InventoryListener -- KCD2 WHGame.dll 1.5.6 (kd7u)
// -----------------------------------------------
// Inventory-change listener interface (vtable only).
// The standalone vtable at 0x183A65058 has eight validated entries; slots [0..6]
// are default no-ops and [7] is the deleting destructor.

namespace wh::entitymodule {

class C_Item;

class I_InventoryListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_InventoryListener;
    virtual void InventoryListenerUnk0(C_Item* item) {}  // [0] [U role]
    virtual void InventoryListenerUnk1(void* value) {}   // [1] [U role]
    virtual void InventoryListenerUnk2(void* value) {}   // [2] [U role]
    virtual void InventoryListenerUnk3() {}              // [3] [U signature]
    virtual void InventoryListenerUnk4() {}              // [4] [U signature]
    virtual void InventoryListenerUnk5() {}              // [5] [U signature]
    virtual void InventoryListenerUnk6() {}              // [6] [U signature]
    virtual ~I_InventoryListener() = default;             // [7] sub_1829A820C
};
static_assert(sizeof(I_InventoryListener) == 0x08, "I_InventoryListener must be vptr-only");

}  // namespace wh::entitymodule
