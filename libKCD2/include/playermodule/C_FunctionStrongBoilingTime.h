#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionStrongBoilingTime -- recipe-DSL builtin `StrongBoilingTime('guid', N)`
// (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DDEBB0 (name @0x184DDEBC0).  GetName thunk sub_180781D30 -> "StrongBoilingTime".
// Evaluate [4] = 0x182E2BB6C -> core sub_182E2BB84(ctx, mode=1): same as WeakBoilingTime but over
// m_strongBoilSeconds (accrued while the pot is StrongBoiling, i.e. bellows-boosted fire).
// Fail feedback: 9 Overcooked / 10 Underdone; if the WEAK timer would match N instead ->
// 14 LowCookingTemperature.

namespace wh::playermodule {

class C_FunctionStrongBoilingTime : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionStrongBoilingTime;
};

}  // namespace wh::playermodule
