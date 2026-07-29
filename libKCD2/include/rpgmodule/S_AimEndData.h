#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_AimEndData -- payload of C_AimEndCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD4D24 tracked-alloc 32; same copy shape as
// S_AimBeginData: _OWORD @+0x08 + qword @+0x18).  Field names proposed [U].

namespace wh::rpgmodule {

struct S_AimEndData {
    uint64_t m_a;   // +0x00  WUID [proposed]
    uint64_t m_b;   // +0x08  WUID/handle [proposed]
    uint64_t m_c;   // +0x10  qword [U role]
};
static_assert(sizeof(S_AimEndData) == 0x18, "S_AimEndData must be 0x18 (24-byte ctor copy)");

}  // namespace wh::rpgmodule
