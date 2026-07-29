#pragma once
#include "C_DialogueNodeEEFunction.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_FunctionPort -- "Port" dialogue expression intrinsic
// (KCD2 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184B670B8; vtable 0x183E6CFC0 (8 slots). GetName = "Port". Evaluate
// sub_1803BC574: arg = index -> port name; resolves the named Port on the active
// dialog's expression engine (sub_1803BD13C). Branch-condition intrinsic.

namespace wh::dialogmodule::data {

class C_FunctionPort : public C_DialogueNodeEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionPort;
};
static_assert(sizeof(C_FunctionPort) == 0x08, "C_FunctionPort adds no data (alloc 8)");

}  // namespace wh::dialogmodule::data
