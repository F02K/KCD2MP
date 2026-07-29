#pragma once
#include "C_Effect.h"

// -----------------------------------------------
// wh::conceptmodule::C_SoulEffect -- soul-targeted concept effect (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Ctor sub_181E2E290: C_Effect chain then own vftable only -- adds NO data (derived
// rpgmodule::C_BuffEffect members start at +0x88 = sizeof(C_Effect)).

namespace wh::conceptmodule {

class C_SoulEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_conceptmodule_C_SoulEffect;
};
static_assert(sizeof(C_SoulEffect) == 0x88, "conceptmodule::C_SoulEffect must be 0x88");

}  // namespace wh::conceptmodule
