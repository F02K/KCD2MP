#pragma once
#include <cstdint>

// -----------------------------------------------
// IFlowNode / CFlowBaseNodeInternal / CFlowBaseNode<N> -- CryEngine FlowGraph node bases
// (KCD2 WHGame.dll 1.5.6, kd7u). Minimal layout stubs.
// -----------------------------------------------
// RTTI base chain of the wh flow nodes (environmentmodule CFlowNode_Weather /
// CFlowNodeHDRPreset): CFlowBaseNode<0> -> CFlowBaseNodeInternal -> IFlowNode, all mdisp 0
// (one collapsed vtable). Layout evidence: the node factories sub_182ADABE8 / sub_182ADAB70
// alloc 0x10 and write {vftable, dword 0 @+0x08} -- IFlowNode contributes the vptr,
// CFlowBaseNodeInternal the refcount, CFlowBaseNode<N> adds nothing.
// IFlowNode is an interfuscator-shuffled SDK interface: slot ORDER is NOT certified, only
// the vptr matters for layout. Do NOT call through these stubs.

namespace Offsets {

struct IFlowNode {
    virtual void _vf0() = 0;   // slot roles not certified (interfuscator-shuffled)
};
static_assert(sizeof(IFlowNode) == 0x08, "IFlowNode is a vptr-only interface");

// GameSDK-style refcounted flow-node base: adds only the refcount.
struct CFlowBaseNodeInternal : IFlowNode {
    int32_t  m_refs;    // +0x08  factory writes 0
    uint32_t _pad0C;    // +0x0C
};
static_assert(sizeof(CFlowBaseNodeInternal) == 0x10, "CFlowBaseNodeInternal must be 0x10");

// Clone-type-tagged base (0 = singleton node). Adds no data.
template <int TCloneType>
struct CFlowBaseNode : CFlowBaseNodeInternal {};

}  // namespace Offsets
