#pragma once
#include "../Offsets/vtables/CFlowBaseNode.h"

// -----------------------------------------------
// wh::environmentmodule::CFlowNode_Weather -- FlowGraph weather-control node
// (KCD2 1.5.6, kd7u).  sizeof 0x10 (alloc 16).
// -----------------------------------------------
// Own vtable 0x183ECF530; Clone factory sub_182ADABE8 (alloc, vftable, vfunc[1]=AddRef).
// Singleton flow node (CFlowBaseNode<0>) with no data beyond the refcount.
// Cheat-relevant: FlowGraph-level weather switching.

namespace wh::environmentmodule {

class CFlowNode_Weather : public Offsets::CFlowBaseNode<0> {};
static_assert(sizeof(CFlowNode_Weather) == 0x10, "CFlowNode_Weather must be 0x10 (alloc 16)");

}  // namespace wh::environmentmodule
