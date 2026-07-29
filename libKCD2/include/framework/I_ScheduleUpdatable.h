#pragma once
#include <cstdint>

namespace wh::framework {

// wh::framework::I_ScheduleUpdatable -- KCD2 WHGame.dll 1.5.6 (kd7u).
// Pure one-slot interface. C_InventorySoul implements the slot at sub_1808F2E88.
class I_ScheduleUpdatable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ScheduleUpdatable;
    virtual void ScheduleUpdate(std::int32_t value) = 0;  // [0] [U parameter semantics]
};
static_assert(sizeof(I_ScheduleUpdatable) == 0x08, "I_ScheduleUpdatable must be vptr-only");

}  // namespace wh::framework
