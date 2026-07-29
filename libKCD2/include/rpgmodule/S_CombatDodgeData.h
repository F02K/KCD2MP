#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_CombatDodgeData -- payload of C_CombatDodgeCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x28 (PROVEN: ctor sub_1816490FC tracked-alloc 48 = 8+40; copies _OWORD@0 +
// _OWORD@0x10 + qword@0x20).  Create-site sub_181648F28 feeds +0x00/+0x08 into
// sub_181F985D0(mgr+56,&key) lookups and reads the bool at +0x20.

namespace wh::rpgmodule {

struct S_CombatDodgeData {
    uint64_t m_attacker;    // +0x00  handle/WUID [proposed]
    uint64_t m_dodger;      // +0x08  handle/WUID [proposed]
    uint64_t m_c;           // +0x10  } copied, not read at create-site [U]
    uint64_t m_d;           // +0x18  }
    uint8_t  m_flag;        // +0x20  bool [proposed]
    uint8_t  _pad21[7];     // +0x21
};
static_assert(sizeof(S_CombatDodgeData) == 0x28, "S_CombatDodgeData must be 0x28 (40-byte ctor copy)");

}  // namespace wh::rpgmodule
