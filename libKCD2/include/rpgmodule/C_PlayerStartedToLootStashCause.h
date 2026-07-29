#pragma once
#include "C_Cause.h"
#include "S_PlayerStartedToLootStashData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PlayerStartedToLootStashCause -- "PlayerStartedToLootStash" event
// cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CD5B6C tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5F270, vtable rva 0x3F274B8 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial),
// [1] nullsub, [2] Accept -> visitor+0x300, [3] GetName -> "PlayerStartedToLootStash"
// (verified).  make+post shape; caller sub_182CFA714.

namespace wh::rpgmodule {

class C_PlayerStartedToLootStashCause : public C_Cause<S_PlayerStartedToLootStashData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerStartedToLootStashCause;
};
static_assert(sizeof(C_PlayerStartedToLootStashCause) == 0x18,
              "C_PlayerStartedToLootStashCause must be 0x18 (alloc 24 at sub_182CD5B6C)");

}  // namespace wh::rpgmodule
