#pragma once
#include "C_Cause.h"
#include "S_EncumberanceWalkDistanceReachedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_EncumberanceWalkDistanceReachedCause --
// "EncumberanceWalkDistanceReached" event cause (game's own spelling) (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_182D18F38 tracked-alloc 16, += 0x10 @0x182D18F78).
// RTTI TD rva 0x4D71920, vtable rva 0x3F45648 (4 slots): [0] dtor rva 0x9DC2C0 (ICF
// trivial), [1] nullsub, [2] Accept rva 0x2CA92FC -> visitor+0x148, [3] GetTypeName rva
// 0x2CAC7A4 -> "EncumberanceWalkDistanceReached" (verified).  Pattern-B ctor; create-site
// sub_180729BDC (call @0x18072A273).

namespace wh::rpgmodule {

class C_EncumberanceWalkDistanceReachedCause
    : public C_Cause<S_EncumberanceWalkDistanceReachedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EncumberanceWalkDistanceReachedCause;
};
static_assert(sizeof(C_EncumberanceWalkDistanceReachedCause) == 0x10,
              "C_EncumberanceWalkDistanceReachedCause must be 0x10 (alloc 16 at sub_182D18F38)");

}  // namespace wh::rpgmodule
