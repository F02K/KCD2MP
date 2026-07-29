#pragma once
#include "C_Cause.h"
#include "S_BarberServiceUsedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_BarberServiceUsedCause -- "BarberServiceUsed" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: factory sub_182D18DEC tracked-alloc 16).  RTTI TD rva 0x4D71B18,
// vtable rva 0x3F45848 (4 slots): [0] dtor 0x1809DC2C0 (ICF trivial), [1] nullsub,
// [2] Accept 0x1809DC38C -> visitor+120, [3] GetTypeName 0x182D27D6C ->
// "BarberServiceUsed" (verified).  Event-factory shape.

namespace wh::rpgmodule {

class C_BarberServiceUsedCause : public C_Cause<S_BarberServiceUsedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BarberServiceUsedCause;
};
static_assert(sizeof(C_BarberServiceUsedCause) == 0x10,
              "C_BarberServiceUsedCause must be 0x10 (alloc 16 at sub_182D18DEC)");

}  // namespace wh::rpgmodule
