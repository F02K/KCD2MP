#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionContainsIngredient -- recipe-DSL builtin `IngredientCount('guid')`
// (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DE1300 (name @0x184DE1310).  GetName thunk sub_180781CA0 -> "IngredientCount".
// Evaluate [4] = 0x182E2B304: counts brew records whose GUID matches the arg (sub_1823CD2D4,
// alias-aware) and PUSHES THE INTEGER COUNT onto the operand stack (the outer ==/>= compares it).
// On mismatch writes feedback 11 SmallerAmounts / 12 BiggerAmounts (= (expected<actual)+11); if
// zero found clears brewState+0x2D (hasAnyCorrectIngredient) -> aggregate WrongIngredients.

namespace wh::playermodule {

class C_FunctionContainsIngredient : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionContainsIngredient;
};

}  // namespace wh::playermodule
