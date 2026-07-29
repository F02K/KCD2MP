#pragma once

// -----------------------------------------------
// wh::entitymodule::C_ItemHolder -- KCD2 WHGame.dll 1.5.6 (kd7u)
// -----------------------------------------------
// Near-empty polymorphic base of C_InventoryBase (and thus C_Inventory). Provides the shared primary
// vtable pointer and only registers/unregisters itself in a global item-holder registry (dtor
// sub_18046653C) -- no own instance data. sizeof 8 (vtable only).

namespace wh::entitymodule {

class C_ItemHolder {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemHolder;
    virtual ~C_ItemHolder() = default;   // [0]
};

}  // namespace wh::entitymodule
