#pragma once
#include "C_SoulStateEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_AlcoholismValueEffect -- change soul alcoholism (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184CEA2D0  vtable 0x183F05A88.  sizeof 0x30 (PROVEN: factory sub_182BD905C
// operator new(48)).  : C_SoulStateEffect with m_valueId preset to E_SoulState::Alcoholism (5);
// no members of its own.

namespace wh::rpgmodule {

class C_AlcoholismValueEffect : public C_SoulStateEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlcoholismValueEffect;
};
static_assert(sizeof(C_AlcoholismValueEffect) == 0x30, "C_AlcoholismValueEffect must be 0x30 (operator new(48))");

}  // namespace wh::rpgmodule
