#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_FastTravelStartedData -- payload of C_FastTravelStartedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD52E8 tracked-alloc 24; identical layout to
// S_FastTravelEndedData: qword @0x182CD534D + dword @0x182CD5355).  Create-site
// sub_182CF9824 builds src from *a2 + *(uint*)(a2+8).

namespace wh::rpgmodule {

struct S_FastTravelStartedData {
    uint64_t m_target;    // +0x00  travel-target handle [proposed]
    int32_t  m_id;        // +0x08  id/type [proposed]
    uint8_t  _pad0C[4];   // +0x0C
};
static_assert(sizeof(S_FastTravelStartedData) == 0x10, "S_FastTravelStartedData must be 0x10");

}  // namespace wh::rpgmodule
