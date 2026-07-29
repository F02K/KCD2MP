#pragma once
#include "C_Cause.h"
#include "S_PlayerIsActiveInCombatData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PlayerIsActiveInCombatCause -- "PlayerIsActiveInCombat" event cause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_1817D5F60 tracked-alloc 16, += 0x10; 1-byte bool payload
// + 7B tail pad).  RTTI TD rva 0x4D8EF70, vtable rva 0x3C1BC58 (4 slots): [0] dtor
// 0x1809DC2C0 (ICF trivial), [1] nullsub, [2] Accept -> visitor+0x258, [3] GetName ->
// "PlayerIsActiveInCombat" (verified).  make+post shape; poller sub_1809D12C8 latches
// byte_18548E91D.

namespace wh::rpgmodule {

class C_PlayerIsActiveInCombatCause : public C_Cause<S_PlayerIsActiveInCombatData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerIsActiveInCombatCause;
};
static_assert(sizeof(C_PlayerIsActiveInCombatCause) == 0x10,
              "C_PlayerIsActiveInCombatCause must be 0x10 (alloc 16 at sub_1817D5F60)");

}  // namespace wh::rpgmodule
