#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::entitymodule::E_WeaponEquipSlot -- KCD2 WHGame.dll 1.5.6.  Fixed physical equipment slots.
// -----------------------------------------------
// Standalone native enum (not EquipmentSlotId and not E_HandSlot). Enumerators are certified from
// RTTR registration sub_180CFFDC8. C_EquipmentManager stores one C_Item* for each value at +0x98.

namespace wh::entitymodule {

enum class E_WeaponEquipSlot : std::int32_t {
    PrimaryMainHand   = 0,
    PrimaryOffHand    = 1,
    SecondaryMainHand = 2,
    SecondaryOffHand  = 3,
    Oversized         = 4,
    OversizedOff      = 5,
    Torch             = 6,
    Dagger            = 7,
};

static_assert(sizeof(E_WeaponEquipSlot) == 0x04, "E_WeaponEquipSlot must be 0x04");

}  // namespace wh::entitymodule
