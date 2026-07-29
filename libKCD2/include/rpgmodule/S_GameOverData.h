#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_GameOverData -- payload of C_GameOverCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: ctor sub_182CD539C tracked-alloc 16; dword *(obj+8)=*a3
// @0x182CD53FC).  Create-site sub_181FD1CF0 passes *a1.

namespace wh::rpgmodule {

struct S_GameOverData {
    int32_t m_reason;     // +0x00  game-over reason/enum [proposed]
    uint8_t _pad04[4];    // +0x04
};
static_assert(sizeof(S_GameOverData) == 0x08, "S_GameOverData must be 0x08");

}  // namespace wh::rpgmodule
