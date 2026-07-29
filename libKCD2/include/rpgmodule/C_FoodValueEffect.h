#pragma once
#include "C_SoulStateEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_FoodValueEffect -- change soul food level (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5FF98  vtable 0x183F277C8.  sizeof 0x30 (PROVEN: factory sub_182CD408C
// operator new(48)).  : C_SoulStateEffect with m_valueId preset to E_SoulState::Food (3);
// no members of its own.

namespace wh::rpgmodule {

class C_FoodValueEffect : public C_SoulStateEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FoodValueEffect;
};
static_assert(sizeof(C_FoodValueEffect) == 0x30, "C_FoodValueEffect must be 0x30 (operator new(48))");

}  // namespace wh::rpgmodule
