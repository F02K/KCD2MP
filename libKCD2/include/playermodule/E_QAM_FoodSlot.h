#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_QAM_FoodSlot -- KCD2 WHGame.dll 1.5.6.  Enum wrapper, Type : int32.
// -----------------------------------------------
// Player quick-access food/consumable assignments. Native string conversion is sub_1808F8D54.

namespace wh::playermodule {

struct E_QAM_FoodSlot {
    enum Type : std::int32_t {
        Slot_1 = 0,
        Slot_2 = 1,
        Slot_3 = 2,
        Slot_4 = 3,
    };
};

static_assert(sizeof(E_QAM_FoodSlot::Type) == 0x04,
    "E_QAM_FoodSlot::Type must be 0x04");

}  // namespace wh::playermodule
