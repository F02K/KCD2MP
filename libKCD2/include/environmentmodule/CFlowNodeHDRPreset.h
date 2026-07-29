#pragma once
#include "../Offsets/vtables/CFlowBaseNode.h"

// -----------------------------------------------
// wh::environmentmodule::CFlowNodeHDRPreset -- FlowGraph HDR-preset node
// (KCD2 1.5.6, kd7u).  sizeof 0x10 (alloc 16).
// -----------------------------------------------
// Own vtable 0x183ECF4D0; Clone factory sub_182ADAB70. Same singleton flow-node shape as
// CFlowNode_Weather (CFlowBaseNode<0> + refcount only).

namespace wh::environmentmodule {

class CFlowNodeHDRPreset : public Offsets::CFlowBaseNode<0> {};
static_assert(sizeof(CFlowNodeHDRPreset) == 0x10, "CFlowNodeHDRPreset must be 0x10 (alloc 16)");

}  // namespace wh::environmentmodule
