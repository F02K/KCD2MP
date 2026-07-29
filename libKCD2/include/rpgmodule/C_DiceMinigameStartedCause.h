#pragma once
#include "C_Cause.h"
#include "S_DiceMinigameStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_DiceMinigameStartedCause -- "DiceMinigameStarted" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD50EC tracked-alloc 16, += 0x10 @0x182CD512C).
// RTTI TD rva 0x4D5D6E0, vtable rva 0x3F273A0 (4 slots): [0] dtor rva 0x9DC2C0 (ICF
// trivial), [1] nullsub, [2] Accept rva 0x2CA92E8 -> visitor+0xD0, [3] GetTypeName rva
// 0x2CAC6BC -> "DiceMinigameStarted" (verified).  Pattern-B ctor; create-site
// sub_182CF9638.

namespace wh::rpgmodule {

class C_DiceMinigameStartedCause : public C_Cause<S_DiceMinigameStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DiceMinigameStartedCause;
};
static_assert(sizeof(C_DiceMinigameStartedCause) == 0x10,
              "C_DiceMinigameStartedCause must be 0x10 (alloc 16 at sub_182CD50EC)");

}  // namespace wh::rpgmodule
