#pragma once
#include "C_Cause.h"
#include "S_SharpeningVictoryData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SharpeningVictoryCause -- "SharpeningVictory" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: create fn sub_182CD620C tracked-alloc 24).  RTTI TD rva 0x4D5DF38,
// vtable rva 0x3F272B0 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial), [1] nullsub,
// [2] Accept rva 0x2CA9460 -> visitor+448, [3] GetName rva 0x2CAD868 -> "SharpeningVictory"
// (verified).  Factory caller sub_182CF19E0.

namespace wh::rpgmodule {

class C_SharpeningVictoryCause : public C_Cause<S_SharpeningVictoryData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SharpeningVictoryCause;
};
static_assert(sizeof(C_SharpeningVictoryCause) == 0x18,
              "C_SharpeningVictoryCause must be 0x18 (alloc 24 at sub_182CD620C)");

}  // namespace wh::rpgmodule
