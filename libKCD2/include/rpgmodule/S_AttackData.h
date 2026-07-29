#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_AttackData -- payload of C_AttackCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x28 (PROVEN: factory sub_180C64E80 tracked-alloc 48 = 8+40; copies _OWORD
// @+0x08 (0x180C64EE7), _OWORD @+0x18 (0x180C64EEF), qword @+0x28 (0x180C64EF8)).
// First pair proposed attacker/target WUIDs; remaining fields [U].

namespace wh::rpgmodule {

struct S_AttackData {
    uint64_t m_attacker;   // +0x00  attacker WUID [proposed]
    uint64_t m_target;     // +0x08  target WUID [proposed]
    uint64_t m_c;          // +0x10  } second _OWORD [U roles]
    uint64_t m_d;          // +0x18  }
    uint64_t m_e;          // +0x20  qword [U role]
};
static_assert(sizeof(S_AttackData) == 0x28, "S_AttackData must be 0x28 (40-byte ctor copy)");

}  // namespace wh::rpgmodule
