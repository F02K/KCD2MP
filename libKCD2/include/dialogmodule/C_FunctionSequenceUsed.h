#pragma once
#include "C_DialogueNodeEEFunction.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_FunctionSequenceUsed -- "SequenceUsed" dialogue expression
// intrinsic (KCD2 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184B67150; vtable 0x183E6CF30 (8 slots). GetName = "SequenceUsed". Evaluate
// sub_181303510: arg = sequence name; returns bool "was that sequence used by the
// current role". Branch-condition intrinsic (drives don't-repeat-lines logic).

namespace wh::dialogmodule::data {

class C_FunctionSequenceUsed : public C_DialogueNodeEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionSequenceUsed;
};
static_assert(sizeof(C_FunctionSequenceUsed) == 0x08, "C_FunctionSequenceUsed adds no data (alloc 8)");

}  // namespace wh::dialogmodule::data
