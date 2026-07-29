#pragma once
#include "C_SoulEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_RestartStaminaRegenCooldownEffect -- restart the stamina-regen cooldown
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E2E0  vtable 0x183F1EC30.  sizeof 0x18 (PROVEN: factory sub_182CD45B8
// operator new(24)).  Pure C_SoulEffect -- no members of its own.

namespace wh::rpgmodule {

class C_RestartStaminaRegenCooldownEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RestartStaminaRegenCooldownEffect;
};
static_assert(sizeof(C_RestartStaminaRegenCooldownEffect) == 0x18,
              "C_RestartStaminaRegenCooldownEffect must be 0x18 (operator new(24))");

}  // namespace wh::rpgmodule
