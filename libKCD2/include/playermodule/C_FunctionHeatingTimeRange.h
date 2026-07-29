#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionHeatingTimeRange -- recipe-DSL builtin `HeatingTimeRange` (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DDEA40 (name @0x184DDEA50).  GetName thunk sub_180781D50 -> "HeatingTimeRange".
// Evaluate [4] = 0x182E2B690 (range variant of the heat test; also uses the skill tolerance).
// UNUSED by the shipped recipe tables (available for modded recipes).

namespace wh::playermodule {

class C_FunctionHeatingTimeRange : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionHeatingTimeRange;
};

}  // namespace wh::playermodule
