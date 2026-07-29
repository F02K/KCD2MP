#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::entitymodule::E_InteractionType -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum, int32.
// -----------------------------------------------
// Interaction-verb type of the entity interactor system. Read by C_InteractorOverride
// through a C_TypedPortRef<E_InteractionType> and cached as an int32 (@node+0x278,
// converter sub_1817C6664) -- underlying width proven by that cache; ENUMERATOR
// NAMES/VALUES NOT RECOVERED -- do not invent values; extend when certified.

namespace wh::entitymodule {

enum class E_InteractionType : int32_t {};   // enumerators UNVERIFIED

}  // namespace wh::entitymodule
