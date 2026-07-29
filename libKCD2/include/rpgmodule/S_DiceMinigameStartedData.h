#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_DiceMinigameStartedData -- payload of C_DiceMinigameStartedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x08 (PROVEN: ctor sub_182CD50EC tracked-alloc 16; v5[1]=*a3 @0x182CD514D).
// Create-site sub_182CF9638: value = *functor (entity/WUID) [proposed].

namespace wh::rpgmodule {

struct S_DiceMinigameStartedData {
    uint64_t m_player;   // +0x00  entity/WUID [proposed]
};
static_assert(sizeof(S_DiceMinigameStartedData) == 0x08, "S_DiceMinigameStartedData must be 0x08");

}  // namespace wh::rpgmodule
