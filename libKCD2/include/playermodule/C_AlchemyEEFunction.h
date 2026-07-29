#pragma once
#include "../framework/I_EEFunction.h"

// -----------------------------------------------
// wh::playermodule::C_AlchemyEEFunction -- alchemy expression-engine function base (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_AlchemyEEFunction@playermodule@wh@@ (TD 0x184DE12C0).  Base of the 11 recipe-DSL
// builtin functors (C_Function*), registered as prototypes into the global EE function manager
// (qword_185168B30+0x430) that the Condition parser (sub_1807809A0) resolves identifiers against.
//
// vtable contract = framework::I_EEFunction's 8 slots; the semantic method is Evaluate [4]:
//   Evaluate(this, ctx) with ctx = 3 sources:
//     ctx[0]+0x418 = GUID string pool (char* per argIndex; the '...' literals),
//     ctx[1]       = operand/value stack (pop sub_1803BCC7C),
//     ctx[2]+0x20  = the brew-state eval view: +0x08 {begin,end} DIRECTLY into the per-kind
//                    resource-bucket array (no copy/flatten), +0x10 recipeDef, +0x18/+0x19/+0x1C
//                    feedback out (flag/type/GUID), +0x2C/+0x2D/+0x2E correctness flags.
// Driver: sub_182E2BD10 iterates every recipe's resolved steps and evaluates the compiled trees
// through the RPN walker sub_1803BC690 (wrapper sub_1803BC4D4).
// [Own data members beyond the vptr UNVERIFIED (none observed); no sizeof static_assert.]

namespace wh::playermodule {

class C_AlchemyEEFunction : public wh::framework::I_EEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AlchemyEEFunction;
};

}  // namespace wh::playermodule
