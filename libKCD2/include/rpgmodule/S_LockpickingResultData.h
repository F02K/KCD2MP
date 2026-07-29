#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_LockpickingResultData -- payload of C_LockpickingResultCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_180898A9C tracked-alloc 24; _OWORD @+0x08).  Caller
// sub_18089832C.  Likely lock handle + success/quality; split UNVERIFIED.

namespace wh::rpgmodule {

struct S_LockpickingResultData {
    uint64_t m_lock;     // +0x00  lock handle [U]
    float    m_result;    // +0x08  lockpicking result value (caller sub_18089832C builds {lock, float a4, byte a5})
    bool     m_success;   // +0x0C  gates the success/XP path
    uint8_t  _pad0D[3];   // +0x0D
};
static_assert(sizeof(S_LockpickingResultData) == 0x10, "S_LockpickingResultData must be 0x10");

}  // namespace wh::rpgmodule
