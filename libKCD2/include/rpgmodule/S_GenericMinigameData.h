#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_GenericMinigameData -- payload of C_GenericMinigameCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: ctor sub_182CD5440 tracked-alloc 16; byte *(obj+8)=*a3
// @0x182CD54A0).  Create-site sub_182CF1DD8 (RNG-gated) passes constant 7.

namespace wh::rpgmodule {

struct S_GenericMinigameData {
    uint8_t m_minigameType;   // +0x00  playermodule::E_MinigameType::Type truncated to a byte (RTTR reg sub_180227FD0; create sub_182CF1DD8 passes Dice=7)
    uint8_t _pad01[7];        // +0x01
};
static_assert(sizeof(S_GenericMinigameData) == 0x08, "S_GenericMinigameData must be 0x08");

}  // namespace wh::rpgmodule
