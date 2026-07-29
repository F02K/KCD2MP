#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionIsMilled -- recipe-DSL builtin `IsMilled('guid', N)` (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DE1280 (name @0x184DE1290).  GetName thunk sub_180781CF0 -> "IsMilled".
// Evaluate [4] = 0x182E2B9A4: AND-reduces (m_flags & Milled) over the GUID-matching records
// (sub_182E2C70C); result = (isMilled == (N==1)) -- N=1 MUST be milled, N=0 must NOT.
// Fail feedback: 6 IngredientNotMilled (wanted milled) / 5 IngredientMilled (wanted unmilled).

namespace wh::playermodule {

class C_FunctionIsMilled : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionIsMilled;
};

}  // namespace wh::playermodule
