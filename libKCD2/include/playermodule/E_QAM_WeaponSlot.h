#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_QAM_WeaponSlot -- KCD2 WHGame.dll 1.5.6.  Enum wrapper, Type : int32.
// -----------------------------------------------
// Player quick-access weapon assignments. Values flatten four Main/Off pairs; they are separate
// from entitymodule::E_WeaponEquipSlot. Native string conversion is sub_1808F87DC.

namespace wh::playermodule {

struct E_QAM_WeaponSlot {
    enum Type : std::int32_t {
        Main_1 = 0,
        Off_1  = 1,
        Main_2 = 2,
        Off_2  = 3,
        Main_3 = 4,
        Off_3  = 5,
        Main_4 = 6,
        Off_4  = 7,
    };
};

static_assert(sizeof(E_QAM_WeaponSlot::Type) == 0x04,
    "E_QAM_WeaponSlot::Type must be 0x04");

}  // namespace wh::playermodule
