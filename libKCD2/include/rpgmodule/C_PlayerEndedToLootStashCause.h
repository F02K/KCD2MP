#pragma once
#include "C_Cause.h"
#include "S_PlayerEndedToLootStashData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PlayerEndedToLootStashCause -- "PlayerEndedToLootStash" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x58 (PROVEN: ctor sub_182CDE9A8 tracked-alloc 88, += 0x58).  RTTI TD rva
// 0x4D5F1C0, vtable rva 0x3F274E0 (4 slots): [0] dtor 0x1819A3DC0 (own body -- releases
// the 64B container), [1] nullsub, [2] Accept -> visitor+0x2F8, [3] GetName ->
// "PlayerEndedToLootStash" (verified).  make-only shape (raw cause returned; caller
// sub_182CFA59C posts separately).

namespace wh::rpgmodule {

class C_PlayerEndedToLootStashCause : public C_Cause<S_PlayerEndedToLootStashData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerEndedToLootStashCause;
};
static_assert(sizeof(C_PlayerEndedToLootStashCause) == 0x58,
              "C_PlayerEndedToLootStashCause must be 0x58 (alloc 88 at sub_182CDE9A8)");

}  // namespace wh::rpgmodule
