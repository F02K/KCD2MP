#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::E_NodeRuntimeState -- concept-node scheduling state
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).
// -----------------------------------------------
// Registered rttr enum "wh::conceptmodule::NodeRuntimeState" (reg fn sub_1800A8990:
// "Awake"=0, "Hibernating"=1; 2 enumerators cross-checked against the mangled
// enumeration_wrapper).  Flipped by C_Node::Hibernate (slot 20, 0x180ACFEE4) /
// WakeUp (slot 21, 0x180695248); gates C_Node::Execute (slot 13, 0x180692060):
// a Hibernating node only runs OnExecute when the firing port's owner allows it.

namespace wh::conceptmodule {

enum class E_NodeRuntimeState : int32_t {
    Awake       = 0,
    Hibernating = 1,   // C_Node ctor default (sub_1806B2744 @0x1806b277d)
};

}  // namespace wh::conceptmodule
