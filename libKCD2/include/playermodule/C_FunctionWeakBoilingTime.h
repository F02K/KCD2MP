#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionWeakBoilingTime -- recipe-DSL builtin `WeakBoilingTime('guid', N)`
// (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DDEB70 (name @0x184DDEB80).  GetName thunk sub_180781D20 -> "WeakBoilingTime".
// Evaluate [4] = 0x182E2BB78 -> core sub_182E2BB84(ctx, mode=0): averages m_weakBoilSeconds over
// the GUID-matching records, converts seconds -> SANDGLASS TURNS (/ the HourglassTimeout RPG
// param 10 s; sub_182E2C110), then the comparator sub_182E2B050 passes iff
// |1 - turns/N| <= tol, where tol = the SKILL-scaled brew tolerance (sub_182E2C594, lerp
// AlchemyToleranceBase..AlchemyToleranceMaxLevel by alchemy level) -- the game's one per-operation
// accuracy lever.  Fail feedback: 9 Overcooked / 10 Underdone; if the STRONG timer would match N
// instead -> 13 HighCookingTemperature.

namespace wh::playermodule {

class C_FunctionWeakBoilingTime : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionWeakBoilingTime;
};

}  // namespace wh::playermodule
