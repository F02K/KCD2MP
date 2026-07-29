#pragma once
#include "C_AlchemyEEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_FunctionHeatingTime -- recipe-DSL builtin `HeatingTime` (KCD2 1.5.6).
// -----------------------------------------------
// RTTI TD 0x4DDEB28 (name @0x184DDEB38).  UNUSED by the shipped recipe tables; registered
// prototype only.  Evaluate body not decompiled [semantics UNVERIFIED -- presumably total
// (weak+strong) cook-time variant].

namespace wh::playermodule {

class C_FunctionHeatingTime : public C_AlchemyEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionHeatingTime;
};

}  // namespace wh::playermodule
