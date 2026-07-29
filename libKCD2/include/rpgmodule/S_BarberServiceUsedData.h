#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_BarberServiceUsedData -- payload of C_BarberServiceUsedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_182D18DEC tracked-alloc 16; one qword to cause+0x08).

namespace wh::rpgmodule {

struct S_BarberServiceUsedData {
    uint64_t m_actor;   // +0x00  actor WUID [proposed]
};
static_assert(sizeof(S_BarberServiceUsedData) == 0x08, "S_BarberServiceUsedData must be 0x08");

}  // namespace wh::rpgmodule
