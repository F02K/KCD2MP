#pragma once
#include <cstdint>
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_ProjectileStatusEffect -- projectile state report (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D600F0  vtable 0x183F278E8.  sizeof 0x18 (PROVEN: factory sub_182CD4330
// operator new(24)).  Derives C_Effect DIRECTLY.  Ctor (owner) zero-initializes a single
// WORD at +0x10; payload is filled post-construction [field roles UNVERIFIED].

namespace wh::rpgmodule {

class C_ProjectileStatusEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ProjectileStatusEffect;
    uint16_t m_state10;   // +0x10  ctor 0 [role UNVERIFIED -- two byte flags candidate]
    // +0x12 6-byte tail padding
};
static_assert(sizeof(C_ProjectileStatusEffect) == 0x18,
              "C_ProjectileStatusEffect must be 0x18 (operator new(24))");

}  // namespace wh::rpgmodule
