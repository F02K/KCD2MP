#pragma once
#include "C_Cause.h"
#include "S_GameOverData.h"

// -----------------------------------------------
// wh::rpgmodule::C_GameOverCause -- "GameOver" event cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182CD539C tracked-alloc 16, += 0x10 @0x182CD53DC).
// RTTI TD rva 0x4D5D288, vtable rva 0x3F270F8 (4 slots): [0] dtor rva 0x9DC2C0 (ICF
// trivial), [1] nullsub, [2] Accept rva 0xC6397C -> visitor+0x80, [3] GetTypeName rva
// 0x2CAC974 -> "GameOver" (verified).  Pattern-B ctor; create-site sub_181FD1CF0.

namespace wh::rpgmodule {

class C_GameOverCause : public C_Cause<S_GameOverData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_GameOverCause;
};
static_assert(sizeof(C_GameOverCause) == 0x10, "C_GameOverCause must be 0x10 (alloc 16 at sub_182CD539C)");

}  // namespace wh::rpgmodule
