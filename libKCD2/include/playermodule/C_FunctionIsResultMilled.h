#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionIsResultMilled -- recipe-DSL builtin `IsResultMilled(N)` (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DE1200 (name @0x184DE1210).  GetName thunk sub_180781D80 -> "IsResultMilled".
// Evaluate [4] = 0x182E2BAB4: tests the RESULT record's Milled bit (sub_182E2C6EC(records,1));
// result = (isMilled == (N==1)); clears brewState+0x2E on false.  Used by the gunpowder/ammo
// recipes (mill the distilled result in the mortar -> C_FinishRecipeFromMortarAction).
// Fail feedback: 16 ResultNotMilled / 15 ResultMilled.

namespace wh::playermodule {

class C_FunctionIsResultMilled : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionIsResultMilled;
};

}  // namespace wh::playermodule
