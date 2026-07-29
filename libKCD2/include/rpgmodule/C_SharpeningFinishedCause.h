#pragma once
#include "C_Cause.h"
#include "S_SharpeningFinishedData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SharpeningFinishedCause -- "SharpeningFinished" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x30 (PROVEN: create fn sub_182CD60B0 tracked-alloc 48).  RTTI TD rva 0x4D5E258,
// vtable rva 0x3F27288 (4 slots): [0] dtor rva 0x164FF18, [1] nullsub, [2] Accept rva
// 0x2CA9438 -> visitor+456, [3] GetName rva 0x2CAD780 -> "SharpeningFinished" (verified).
// Factory caller sub_182CFB74C.

namespace wh::rpgmodule {

class C_SharpeningFinishedCause : public C_Cause<S_SharpeningFinishedData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SharpeningFinishedCause;
};
static_assert(sizeof(C_SharpeningFinishedCause) == 0x30,
              "C_SharpeningFinishedCause must be 0x30 (alloc 48 at sub_182CD60B0)");

}  // namespace wh::rpgmodule
