#pragma once
#include "C_Cause.h"
#include "S_SharpeningStartedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SharpeningStartedCause -- "SharpeningStarted" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: create fn sub_182CD6168 tracked-alloc 24).  RTTI TD rva 0x4D5E1B8,
// vtable rva 0x3F27260 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial), [1] nullsub,
// [2] Accept rva 0x2CA944C -> visitor+480, [3] GetName rva 0x2CAD7F4 -> "SharpeningStarted"
// (verified).  Factory caller sub_182CFB970.

namespace wh::rpgmodule {

class C_SharpeningStartedCause : public C_Cause<S_SharpeningStartedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SharpeningStartedCause;
};
static_assert(sizeof(C_SharpeningStartedCause) == 0x18,
              "C_SharpeningStartedCause must be 0x18 (alloc 24 at sub_182CD6168)");

}  // namespace wh::rpgmodule
