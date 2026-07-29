#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SleepwalkingTeleportUsedData -- payload of
// C_SleepwalkingTeleportUsedCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: create fn sub_181FCE750 PLAIN-alloc sub_1804F75C0(16) -- the only
// g11e cause not using the tracked allocator; v5[1]=*a3 @0x181FCE781).  Factory caller
// sub_181FD4B90 (site 0x181FD4D04).

namespace wh::rpgmodule {

struct S_SleepwalkingTeleportUsedData {
    uint64_t m_actor;   // +0x00  teleport/actor handle [proposed]
};
static_assert(sizeof(S_SleepwalkingTeleportUsedData) == 0x08, "S_SleepwalkingTeleportUsedData must be 0x08");

}  // namespace wh::rpgmodule
