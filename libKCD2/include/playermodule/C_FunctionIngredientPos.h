#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionIngredientPos -- recipe-DSL builtin `IngredientPos` (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DE1240 (name @0x184DE1250).  UNUSED by the shipped recipe tables (0 hits in the 270
// step conditions); registered prototype only.  Evaluate body not decompiled [semantics
// UNVERIFIED -- presumably tests an ingredient's add-order position].

namespace wh::playermodule {

class C_FunctionIngredientPos : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionIngredientPos;
};

}  // namespace wh::playermodule
