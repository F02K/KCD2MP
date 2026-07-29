#pragma once
#include "C_ItemEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemHealthValueEffect -- change an item's health/durability
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D60130  vtable 0x183A4B7D8.  sizeof 0x20 (PROVEN: factory sub_1807286E8
// operator new(32)).  Ctor (owner, targetItemWuid, amount).

namespace wh::rpgmodule {

class C_ItemHealthValueEffect : public C_ItemEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemHealthValueEffect;
    float m_amount;     // +0x18  health delta
    // +0x1C 4-byte tail padding (never written by the ctor)
};
static_assert(sizeof(C_ItemHealthValueEffect) == 0x20,
              "C_ItemHealthValueEffect must be 0x20 (operator new(32))");

}  // namespace wh::rpgmodule
