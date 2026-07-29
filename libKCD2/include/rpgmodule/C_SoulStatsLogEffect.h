#pragma once
#include "C_SoulEffect.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulStatsLogEffect -- dump a soul's stats to the log
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E328  vtable 0x183A7BB38.  sizeof 0x18 (PROVEN: factory sub_180B68A1C
// operator new(24)).  Pure C_SoulEffect -- no members of its own.

namespace wh::rpgmodule {

class C_SoulStatsLogEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulStatsLogEffect;
};
static_assert(sizeof(C_SoulStatsLogEffect) == 0x18, "C_SoulStatsLogEffect must be 0x18 (operator new(24))");

}  // namespace wh::rpgmodule
