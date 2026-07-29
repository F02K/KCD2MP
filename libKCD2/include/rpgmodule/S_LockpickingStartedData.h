#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_LockpickingStartedData -- payload of C_LockpickingStartedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_18193569C tracked-alloc 16; v5[1]=*a3).  Caller
// sub_18193561C passes a getter result.

namespace wh::rpgmodule {

struct S_LockpickingStartedData {
    uint64_t m_lock;   // +0x00  lock handle [proposed]
};
static_assert(sizeof(S_LockpickingStartedData) == 0x08, "S_LockpickingStartedData must be 0x08");

}  // namespace wh::rpgmodule
