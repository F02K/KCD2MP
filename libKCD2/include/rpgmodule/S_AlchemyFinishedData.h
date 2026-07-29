#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_AlchemyFinishedData -- payload of C_AlchemyFinishedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_1819357BC tracked-alloc 16; one qword to cause+0x08).
// 8-byte width matches framework::WUID; concrete handle type [U].

namespace wh::rpgmodule {

struct S_AlchemyFinishedData {
    uint64_t m_actor;   // +0x00  actor WUID [proposed]
};
static_assert(sizeof(S_AlchemyFinishedData) == 0x08, "S_AlchemyFinishedData must be 0x08");

}  // namespace wh::rpgmodule
