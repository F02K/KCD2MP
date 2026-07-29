#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_KilledByTimerData -- payload of C_KilledByTimerCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_182D18FDC tracked-alloc 16; v5[1]=*a3).  Caller
// sub_182D28A7C passes *(a2+48).

namespace wh::rpgmodule {

struct S_KilledByTimerData {
    uint64_t m_victim;   // +0x00  victim handle (= *(a2+48)) [proposed]
};
static_assert(sizeof(S_KilledByTimerData) == 0x08, "S_KilledByTimerData must be 0x08");

}  // namespace wh::rpgmodule
