#pragma once
#include "C_SoulEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulPlaceholderEffect -- no-op soul effect placeholder
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E268  vtable 0x183A68D40.  sizeof 0x18 (PROVEN: factory sub_1809DDA90
// operator new(24)).  Pure C_SoulEffect -- no members of its own.

namespace wh::rpgmodule {

class C_SoulPlaceholderEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulPlaceholderEffect;
};
static_assert(sizeof(C_SoulPlaceholderEffect) == 0x18,
              "C_SoulPlaceholderEffect must be 0x18 (operator new(24))");

}  // namespace wh::rpgmodule
