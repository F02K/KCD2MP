#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_ClothingWashedData -- payload of C_ClothingWashedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_1809DCAAC tracked-alloc 16; one qword to cause+0x08).
// Create-site sub_1809DC9D8: value = *(actor->vfunc0()) -> washer WUID [proposed].

namespace wh::rpgmodule {

struct S_ClothingWashedData {
    uint64_t m_washer;   // +0x00  washer-actor WUID [proposed]
};
static_assert(sizeof(S_ClothingWashedData) == 0x08, "S_ClothingWashedData must be 0x08");

}  // namespace wh::rpgmodule
