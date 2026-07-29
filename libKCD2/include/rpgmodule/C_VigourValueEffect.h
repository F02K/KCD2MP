#pragma once
#include "C_SoulStateEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_VigourValueEffect -- change soul vigour (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D5FFD0  vtable 0x183F27830.  sizeof 0x30 (PROVEN: factory sub_182CD4BC0
// operator new(48)).  : C_SoulStateEffect with m_valueId preset to E_SoulState::Vigour (2);
// no members of its own.

namespace wh::rpgmodule {

class C_VigourValueEffect : public C_SoulStateEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_VigourValueEffect;
};
static_assert(sizeof(C_VigourValueEffect) == 0x30, "C_VigourValueEffect must be 0x30 (operator new(48))");

}  // namespace wh::rpgmodule
