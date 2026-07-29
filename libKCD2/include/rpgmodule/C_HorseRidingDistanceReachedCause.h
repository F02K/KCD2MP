#pragma once
#include "C_Cause.h"
#include "S_HorseRidingDistanceReachedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_HorseRidingDistanceReachedCause -- "HorseRidingDistanceReached" event
// cause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_18192C2C4 tracked-alloc 16, += 0x10).  RTTI TD rva
// 0x4D71860, vtable rva 0x3C391C8 (4 slots): [0] dtor rva 0x9DC2C0 (ICF trivial),
// [1] nullsub, [2] Accept rva 0x120705C -> visitor+0x150, [3] GetName rva 0x12071EC ->
// "HorseRidingDistanceReached" (verified).  Caller sub_180729BDC (distance accumulator).

namespace wh::rpgmodule {

class C_HorseRidingDistanceReachedCause : public C_Cause<S_HorseRidingDistanceReachedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_HorseRidingDistanceReachedCause;
};
static_assert(sizeof(C_HorseRidingDistanceReachedCause) == 0x10,
              "C_HorseRidingDistanceReachedCause must be 0x10 (alloc 16 at sub_18192C2C4)");

}  // namespace wh::rpgmodule
