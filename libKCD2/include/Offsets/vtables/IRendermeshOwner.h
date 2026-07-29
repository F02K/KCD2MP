#pragma once

// -----------------------------------------------
// IRendermeshOwner -- WH-ADDED async-render-mesh ownership interface, KCD2 binary slot order
// (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// class (RTTI .?AVIRendermeshOwner@@) -- does NOT exist in stock CE3.8. Secondary base of
// CStatObj at +0x38 (subobject vtable 0x183A2F748, exactly 2 slots, next-COL verified).
// Pure abstract: no standalone default vtable is emitted (the CStatObj dtor restores every
// base vptr EXCEPT +0x38).
//
// Role (from the CStatObj impls): the async render-mesh build pipeline's completion surface.
// Slot [1] stores the mesh size (+0x184), toggles internal bit 0x800000 ("async mesh NOT
// ready") from its first arg, commits the pending render mesh +0x70 -> +0x58 under the SRWLOCK
// (sub_18353E634), and publishes streaming state 2 up the clone chain (sub_1835431EC).
// BOTH METHOD NAMES ARE OURS ([UNVERIFIED]); the producer (caller) was never located.
// Evidence: analysis/mesh_engine_re/cstatobj.md §5.

namespace Offsets {

class IRendermeshOwner {
public:
    virtual void* GetAsyncMeshSource() = 0;                     // [0] returns the smart ptr at owner +0x1F8 (pointee type NOT CMesh -- unidentified)
    virtual void OnRenderMeshReady(bool bSuccess, int nSize) = 0;  // [1] a2!=0 clears "not ready", a2==0 sets it; a3 -> owner +0x184
};
static_assert(sizeof(IRendermeshOwner) == 0x8, "vptr-only interface (no dtor slot!)");

}  // namespace Offsets
