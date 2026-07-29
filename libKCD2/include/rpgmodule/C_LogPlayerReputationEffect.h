#pragma once
#include "C_Effect.h"

// -----------------------------------------------
// wh::rpgmodule::C_LogPlayerReputationEffect -- dump player reputation to the log
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E400  vtable 0x183F1EC80.  sizeof 0x10 (PROVEN: factory sub_180DC91BC
// operator new(16)).  Derives C_Effect DIRECTLY; no members of its own.

namespace wh::rpgmodule {

class C_LogPlayerReputationEffect : public C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LogPlayerReputationEffect;
};
static_assert(sizeof(C_LogPlayerReputationEffect) == 0x10,
              "C_LogPlayerReputationEffect must be 0x10 (operator new(16))");

}  // namespace wh::rpgmodule
