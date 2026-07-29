#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionContainsPotionBase -- recipe-DSL builtin `PotionBase()` (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DDEA90 (name @0x184DDEAA0).  GetName thunk sub_180781D40 -> "PotionBase".
// Evaluate [4] = 0x182E2B480: finds the base record (sub_182E2C5FC: first resource with m_isBase)
// and passes iff its item base-type equals the recipe's BaseMaterial (recipeDef vfunc +0x58).
// On fail clears brewState+0x2C (baseCorrect) -> aggregate feedback RightIngredientsWrongBase.

namespace wh::playermodule {

class C_FunctionContainsPotionBase : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionContainsPotionBase;
};

}  // namespace wh::playermodule
