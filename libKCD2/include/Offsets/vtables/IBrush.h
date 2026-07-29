#pragma once
#include <cstdint>
#include "IRenderNode.h"

// -----------------------------------------------
// IBrush -- brush render-node interface, KCD2 binary slot order (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// struct IBrush : public IRenderNode (RTTI: CBrush BCA [1], mdisp 0). Adds vtable slots
// [73..77] after IRenderNode's 73 -- CBrush's vtable 0x18401FE70 is exactly 78 slots (the
// qword past slot 77 is the next COL, pSelf-verified). No data members (CBrush's first own
// member m_Matrix sits at +0x50 = sizeof(IRenderNode)).
// Slots [73] and [74] are adjacent same-shape (this, bool) bit-setters -- the exact MSVC
// adjacent-overload-reversal hazard shape -- so they carry DISTINCT names by project rule;
// the slot->bit mapping below is hard evidence (0x181AACF00 -> bit0, 0x181AACBA0 -> bit1).
// Evidence: analysis/mesh_engine_re/cbrush_cvegetation.md §1.3.

namespace Offsets {

struct IBrush : public IRenderNode {
    virtual void SetDrawLast(bool bDrawLast) = 0;               // [73] 0x181AACF00: brushFlags(+0xB0) bit0 := b -- the only serialised+cloned 0xB0 bit (bit identity = stock hypothesis)
    virtual void SetPhysicalizationDisabled(bool bDisable) = 0; // [74] 0x181AACBA0: brushFlags bit1 := b -- gates Physicalize/CheckPhysicalized/SetMatrix re-phys everywhere
    virtual float GetMatrixScale() = 0;                         // [75] 0x183493200: length of m_Matrix column 0 (also proves the row-major 16-byte-stride Matrix34 @+0x50)
    virtual void _vf76(void* pUint32Array) = 0;                 // [76] 0x18349CCBC: MOVES a COW uint32 array into +0xD0/+0xD8, invalidates tempdata (SetInstanceData hypothesis -- element semantics unknown)
    virtual bool _vf77() = 0;                                   // [77] 0x180838AE0 `return 0` -- unoverridden default predicate
};
static_assert(sizeof(IBrush) == 0x50, "IBrush adds no data over IRenderNode");

}  // namespace Offsets
