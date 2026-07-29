#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_FastTravelEndedData -- payload of C_FastTravelEndedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD5234 tracked-alloc 24; individually written:
// *(obj+8)=*a3 @0x182CD5299 + *(obj+16)=*(a3+8) dword @0x182CD52A1).

namespace wh::rpgmodule {

struct S_FastTravelEndedData {
    uint64_t m_target;    // +0x00  travel-target handle [proposed]
    int32_t  m_id;        // +0x08  id/type [proposed]
    uint8_t  _pad0C[4];   // +0x0C
};
static_assert(sizeof(S_FastTravelEndedData) == 0x10, "S_FastTravelEndedData must be 0x10");

}  // namespace wh::rpgmodule
