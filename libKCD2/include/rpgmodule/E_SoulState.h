#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_SoulState -- continuous soul-state channels (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI-proven enum (C_SoulValueEffect<E_SoulState, float> instantiation). Values recovered from
// the typed value-effect ctors, each presetting C_SoulStateEffect::m_stateId:
//   C_HealthValueEffect     -> 0        C_ActStaminaValueEffect -> 1
//   C_VigourValueEffect     -> 2        C_FoodValueEffect       -> 3
//   C_AlcoholismValueEffect -> 5
// Value 4 and any higher entries are NOT enumerated (no typed effect observed for them).

namespace wh::rpgmodule {

enum class E_SoulState : uint32_t {
    Health     = 0,
    ActStamina = 1,
    Vigour     = 2,
    Food       = 3,
    // 4 = ? [not enumerated]
    Alcoholism = 5,
};

}  // namespace wh::rpgmodule
