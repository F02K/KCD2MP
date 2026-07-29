#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::entitymodule::E_AuxLightType -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// C_AuxLightAttachment::m_typeId (+0x14, int16). Two values, from the descriptor
// builder sub_1804CE30C: player path stores 10 + "PlayerAuxTorchLight"
// (@0x1804ce405), NPC path stores 20 + "NPCAuxTorchLight" (@0x1804ce34b).
// Enum name and member names [COINED] from those paired string literals.

namespace wh::entitymodule {

enum class E_AuxLightType : int16_t {
    Player = 10,  // "PlayerAuxTorchLight"
    NPC    = 20,  // "NPCAuxTorchLight"
};

}  // namespace wh::entitymodule
