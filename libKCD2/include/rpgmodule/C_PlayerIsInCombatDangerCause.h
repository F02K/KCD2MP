#pragma once
#include "C_Cause.h"
#include "S_PlayerIsInCombatDangerData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PlayerIsInCombatDangerCause -- "PlayerIsInCombatDanger" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_1817D5EC0 tracked-alloc 16, += 0x10; 1-byte bool payload
// + 7B tail pad).  RTTI TD rva 0x4D8E800, vtable rva 0x3C1BC08 (4 slots): [0] dtor
// 0x1809DC2C0 (ICF trivial), [1] nullsub, [2] Accept -> visitor+0x260, [3] GetName ->
// "PlayerIsInCombatDanger" (verified).  make+post shape; poller sub_1809D12C8 latches
// byte_18548E91C.

namespace wh::rpgmodule {

class C_PlayerIsInCombatDangerCause : public C_Cause<S_PlayerIsInCombatDangerData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerIsInCombatDangerCause;
};
static_assert(sizeof(C_PlayerIsInCombatDangerCause) == 0x10,
              "C_PlayerIsInCombatDangerCause must be 0x10 (alloc 16 at sub_1817D5EC0)");

}  // namespace wh::rpgmodule
