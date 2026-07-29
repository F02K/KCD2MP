#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_AlchemyStartedData -- payload of C_AlchemyStartedCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_1808989FC tracked-alloc 16; one qword to cause+0x08).

namespace wh::rpgmodule {

struct S_AlchemyStartedData {
    uint64_t m_actor;   // +0x00  actor WUID [proposed]
};
static_assert(sizeof(S_AlchemyStartedData) == 0x08, "S_AlchemyStartedData must be 0x08");

}  // namespace wh::rpgmodule
