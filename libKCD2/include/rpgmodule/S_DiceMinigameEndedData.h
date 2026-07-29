#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_DiceMinigameEndedData -- payload of C_DiceMinigameEndedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x28 (PROVEN: ctor sub_182CD5034 tracked-alloc 48; _OWORD@0 + _OWORD@0x10 +
// qword@0x20).  Create-site sub_182CF9554 fills the source; semantic names proposed.

namespace wh::rpgmodule {

struct S_DiceMinigameEndedData {
    uint64_t m_q0;          // +0x00  qword (ctor scratch at create-site) [U]
    uint64_t m_player;      // +0x08  handle (= *functor result) [proposed]
    int32_t  m_i10;         // +0x10  int32 (create-site a4) [U role]
    uint8_t  _pad14[4];     // +0x14
    uint64_t m_q18;         // +0x18  qword (create-site a5) [U role]
    int32_t  m_i20;         // +0x20  int32 (create-site a6) [U role]
    uint8_t  _pad24[4];     // +0x24
};
static_assert(sizeof(S_DiceMinigameEndedData) == 0x28, "S_DiceMinigameEndedData must be 0x28 (40-byte ctor copy)");

}  // namespace wh::rpgmodule
