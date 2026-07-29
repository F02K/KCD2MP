#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionIsBaseDistilled -- recipe-DSL builtin `IsBaseDistilled(N)` (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DE11C0 (name @0x184DE11D0).  GetName thunk sub_180781D10 -> "IsBaseDistilled".
// Evaluate [4] = 0x182E2B8E8: tests the base record's Distilled bit (sub_182E2C6EC(records,2));
// result = (isDistilled == (N==1)) -- N=1 the base MUST be distilled (retort + bellows x
// BellowsUseCountToDistill), N=0 must NOT.
// Fail feedback: 8 IngredientNotDistilled / 7 IngredientDistilled.

namespace wh::playermodule {

class C_FunctionIsBaseDistilled : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionIsBaseDistilled;
};

}  // namespace wh::playermodule
