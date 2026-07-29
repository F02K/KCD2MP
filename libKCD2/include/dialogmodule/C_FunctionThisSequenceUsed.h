#pragma once
#include "C_DialogueNodeEEFunction.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_FunctionThisSequenceUsed -- "ThisSequenceUsed" dialogue
// expression intrinsic (KCD2 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184B67070; vtable 0x183E6CF78 (8 slots). GetName = "ThisSequenceUsed".
// Evaluate sub_18152386C: no-arg; tests the CURRENT sequence against the current role
// (via the C_DialogueSequenceHolder context user data) -> bool.

namespace wh::dialogmodule::data {

class C_FunctionThisSequenceUsed : public C_DialogueNodeEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionThisSequenceUsed;
};
static_assert(sizeof(C_FunctionThisSequenceUsed) == 0x08, "C_FunctionThisSequenceUsed adds no data (alloc 8)");

}  // namespace wh::dialogmodule::data
