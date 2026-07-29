#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_BlacksmithingStartedData -- payload of C_BlacksmithingStartedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: factory sub_180B68CCC tracked-alloc 16; one qword to cause+0x08).

namespace wh::rpgmodule {

struct S_BlacksmithingStartedData {
    uint64_t m_actor;   // +0x00  actor WUID [proposed]
};
static_assert(sizeof(S_BlacksmithingStartedData) == 0x08, "S_BlacksmithingStartedData must be 0x08");

}  // namespace wh::rpgmodule
