#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_BowDrawBeginData -- payload of C_BowDrawBeginCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182CD4E3C tracked-alloc 24; one _OWORD copy to
// cause+0x08).  Field roles [U].

namespace wh::rpgmodule {

struct S_BowDrawBeginData {
    uint64_t m_a;   // +0x00  [U role]
    uint64_t m_b;   // +0x08  [U role]
};
static_assert(sizeof(S_BowDrawBeginData) == 0x10, "S_BowDrawBeginData must be 0x10");

}  // namespace wh::rpgmodule
