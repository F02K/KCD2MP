#pragma once
#include "C_SoulStateEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_ActStaminaValueEffect -- change soul act-stamina (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E9F0  vtable 0x183A8BC98.  sizeof 0x30 (PROVEN: factory sub_180C66280
// operator new(48); a second factory sub_180C66334 exists).  : C_SoulStateEffect with m_valueId
// preset to E_SoulState::ActStamina (1); no members of its own.

namespace wh::rpgmodule {

class C_ActStaminaValueEffect : public C_SoulStateEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActStaminaValueEffect;
};
static_assert(sizeof(C_ActStaminaValueEffect) == 0x30, "C_ActStaminaValueEffect must be 0x30 (operator new(48))");

}  // namespace wh::rpgmodule
