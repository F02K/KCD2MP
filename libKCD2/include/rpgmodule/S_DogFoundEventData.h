#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_DogFoundEventData -- payload of C_DogFoundEventCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: ctor sub_182CD5190 tracked-alloc 16; v5[1]=*a3 @0x182CD51F1).
// Create-site sub_182CEEED8 (RNG-gated): value = *functor (dog/entity) [proposed].

namespace wh::rpgmodule {

struct S_DogFoundEventData {
    uint64_t m_dog;   // +0x00  dog/entity WUID [proposed]
};
static_assert(sizeof(S_DogFoundEventData) == 0x08, "S_DogFoundEventData must be 0x08");

}  // namespace wh::rpgmodule
