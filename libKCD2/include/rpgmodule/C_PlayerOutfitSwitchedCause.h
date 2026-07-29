#pragma once
#include "C_Cause.h"
#include "S_PlayerOutfitSwitchedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_PlayerOutfitSwitchedCause -- "PlayerOutfitSwitched" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182D190E8 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D71708, vtable rva 0x3F45590 (4 slots): [0] dtor 0x1814F4A20 (ICF trivial),
// [1] nullsub, [2] Accept -> visitor+0x28, [3] GetName -> "PlayerOutfitSwitched"
// (verified).  make+post shape; caller sub_180D8A154 (fires only when old != new).

namespace wh::rpgmodule {

class C_PlayerOutfitSwitchedCause : public C_Cause<S_PlayerOutfitSwitchedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerOutfitSwitchedCause;
};
static_assert(sizeof(C_PlayerOutfitSwitchedCause) == 0x18,
              "C_PlayerOutfitSwitchedCause must be 0x18 (alloc 24 at sub_182D190E8)");

}  // namespace wh::rpgmodule
