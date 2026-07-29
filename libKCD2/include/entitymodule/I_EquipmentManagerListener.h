#pragma once

// -----------------------------------------------
// wh::entitymodule::I_EquipmentManagerListener -- KCD2 WHGame.dll 1.5.6 (kd7u)
// -----------------------------------------------
// Equipment-change listener interface. Secondary base of C_Player (@+0xA60) and of
// C_UIHudStates (@+0x58). Contributes one vtable pointer.
// Shape re-certified in G3b from the C_UIHudStates subobject vtable 0x183C2ACE0 =
// 4 slots: [0] deleting-dtor thunk 0x18213A4DC, [1] 0x18172ED4C (resolves item by
// WUID a3, tracks it), [2] 0x1816AFB50 (same lookup, writes back item+0x54 value),
// [3] nullsub default. Method names coined from the HudStates bodies; exact
// signatures/direction (equip vs unequip) UNVERIFIED.

namespace wh::entitymodule {

class I_EquipmentManagerListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_EquipmentManagerListener;
    virtual ~I_EquipmentManagerListener() = default;         // [0]
    virtual void OnItemEquipped(void* a2, uint64_t itemWuid) {}    // [1]  (names coined; see header note)
    virtual void OnItemUnequipped(void* a2, uint64_t itemWuid) {}  // [2]
    virtual void OnEquipmentReset() {}                       // [3]  nullsub in C_UIHudStates (name coined)
};

}  // namespace wh::entitymodule
