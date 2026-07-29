#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_BowDrawEndData -- payload of C_BowDrawEndCause (KCD2 WHGame.dll 1.5.6,
// kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_182CD4EE0 tracked-alloc 16; one qword to cause+0x08).

namespace wh::rpgmodule {

struct S_BowDrawEndData {
    uint64_t m_actor;   // +0x00  actor WUID [proposed]
};
static_assert(sizeof(S_BowDrawEndData) == 0x08, "S_BowDrawEndData must be 0x08");

}  // namespace wh::rpgmodule
