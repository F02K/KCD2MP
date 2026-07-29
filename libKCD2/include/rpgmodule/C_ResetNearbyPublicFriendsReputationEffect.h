#pragma once
#include <cstdint>
#include <cstddef>
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_ResetNearbyPublicFriendsReputationEffect -- reset public-friends reputation
// in a radius (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E3B0  vtable 0x183F1EF00.  sizeof 0x20 (ctor sub_182CA7F74 write extent;
// in-place ctor (owner, &center, radius)).  Derives C_Effect directly.

namespace wh::rpgmodule {

class C_ResetNearbyPublicFriendsReputationEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ResetNearbyPublicFriendsReputationEffect;
    Vec3  m_center;   // +0x10  ctor: q+d copy from the Vec3 arg
    float m_radius;   // +0x1C
};
static_assert(sizeof(C_ResetNearbyPublicFriendsReputationEffect) == 0x20,
              "C_ResetNearbyPublicFriendsReputationEffect must be 0x20 (ctor write extent)");
static_assert(offsetof(C_ResetNearbyPublicFriendsReputationEffect, m_radius) == 0x1C, "radius at 0x1C");

}  // namespace wh::rpgmodule
