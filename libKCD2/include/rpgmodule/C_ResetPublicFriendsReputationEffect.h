#pragma once
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_ResetPublicFriendsReputationEffect -- reset public-friends reputation
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E360  vtable 0x18472F3F8.  sizeof 0x10 (PROVEN: factory sub_182C9BB80
// operator new(16)).  Derives C_Effect DIRECTLY (no target member -- just owner registration).
// Notable: KCD1's reputation_change table had NO "ResetPublicFriends" entry (see project notes);
// KCD2 implements it as this dedicated effect pair instead.

namespace wh::rpgmodule {

class C_ResetPublicFriendsReputationEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ResetPublicFriendsReputationEffect;
};
static_assert(sizeof(C_ResetPublicFriendsReputationEffect) == 0x10,
              "C_ResetPublicFriendsReputationEffect must be 0x10 (operator new(16))");

}  // namespace wh::rpgmodule
