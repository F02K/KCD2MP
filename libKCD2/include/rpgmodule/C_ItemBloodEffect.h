#pragma once
#include "C_ItemEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemBloodEffect -- change an item's blood/dirt amount
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D600B8  vtable 0x183A8ACB0.  sizeof 0x20 (PROVEN: factory sub_180C61B44
// operator new(32)).  Ctor (owner, targetItemWuid, amount) -- same shape as
// C_ItemHealthValueEffect.

namespace wh::rpgmodule {

class C_ItemBloodEffect : public C_ItemEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemBloodEffect;
    float m_amount;     // +0x18  blood delta
    // +0x1C 4-byte tail padding (never written by the ctor)
};
static_assert(sizeof(C_ItemBloodEffect) == 0x20, "C_ItemBloodEffect must be 0x20 (operator new(32))");

}  // namespace wh::rpgmodule
