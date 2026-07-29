#pragma once
#include "C_DialogueNodeEEFunction.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_FunctionEnum -- "Enum" dialogue expression intrinsic
// (KCD2 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184B67198; vtable 0x183E6D008 (8 slots). GetName = "Enum". Evaluate
// sub_18282C384: arg = index -> enum name; resolves an enum literal to its int value
// (sub_180C83A9C). Branch-condition intrinsic.

namespace wh::dialogmodule::data {

class C_FunctionEnum : public C_DialogueNodeEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionEnum;
};
static_assert(sizeof(C_FunctionEnum) == 0x08, "C_FunctionEnum adds no data (alloc 8)");

}  // namespace wh::dialogmodule::data
