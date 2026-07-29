#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_BlacksmithingLifetimeData -- payload of C_BlacksmithingLifetimeCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_180B682AC tracked-alloc 24; one _OWORD copy to
// cause+0x08).  Field roles [U].

namespace wh::rpgmodule {

struct S_BlacksmithingLifetimeData {
    uint64_t m_a;   // +0x00  [U role]
    uint64_t m_b;   // +0x08  [U role]
};
static_assert(sizeof(S_BlacksmithingLifetimeData) == 0x10, "S_BlacksmithingLifetimeData must be 0x10");

}  // namespace wh::rpgmodule
