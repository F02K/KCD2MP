#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_BlockingData -- payload of C_BlockingCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_181635788 tracked-alloc 32; _OWORD @+0x08 + qword
// @+0x18).  Field roles [U].

namespace wh::rpgmodule {

struct S_BlockingData {
    uint64_t m_a;   // +0x00  } _OWORD [U roles]
    uint64_t m_b;   // +0x08  }
    uint64_t m_c;   // +0x10  qword [U role]
};
static_assert(sizeof(S_BlockingData) == 0x18, "S_BlockingData must be 0x18 (24-byte ctor copy)");

}  // namespace wh::rpgmodule
