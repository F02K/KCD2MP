#pragma once
#include "C_Cause.h"
#include "S_HerbsCollectedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_HerbsCollectedCause -- "HerbsCollected" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: factory sub_182CD54E4 tracked-alloc 24, += 0x18).  RTTI TD rva
// 0x4D5DFE0, vtable rva 0x3F27440 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x2CA9320 -> visitor+0x1A8, [3] GetName rva 0x2CACA5C ->
// "HerbsCollected" (verified).  Caller sub_181FD48A0 (perk "inventory_perkLuckyFind"
// reward path).

namespace wh::rpgmodule {

class C_HerbsCollectedCause : public C_Cause<S_HerbsCollectedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_HerbsCollectedCause;
};
static_assert(sizeof(C_HerbsCollectedCause) == 0x18, "C_HerbsCollectedCause must be 0x18 (alloc 24 at sub_182CD54E4)");

}  // namespace wh::rpgmodule
