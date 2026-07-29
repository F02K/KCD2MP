#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionBoilingTimeRange -- recipe-DSL builtin `BoilingTimeRange` (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DDEAE0 (name @0x184DDEAF0).  GetName thunk sub_180781D70 -> "BoilingTimeRange".
// Evaluate [4] = 0x182E2B18C (range variant of the boil test; also uses the skill tolerance).
// UNUSED by the shipped recipe tables (available for modded recipes).

namespace wh::playermodule {

class C_FunctionBoilingTimeRange : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionBoilingTimeRange;
};

}  // namespace wh::playermodule
