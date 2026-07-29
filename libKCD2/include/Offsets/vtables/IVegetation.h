#pragma once
#include "IRenderNode.h"

// -----------------------------------------------
// IVegetation -- vegetation render-node interface, KCD2 binary slot order
// (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// struct IVegetation : public IRenderNode (RTTI: CVegetation BCA [1], mdisp 0). Adds exactly
// ONE virtual after IRenderNode's 73 -- CVegetation's vtable 0x183A33D08 is 74 slots (COL
// boundary pSelf-verified). No data members.
// Evidence: analysis/mesh_engine_re/cbrush_cvegetation.md §2.3.

namespace Offsets {

struct IVegetation : public IRenderNode {
    // [73] 0x181AAD2E0: `return (float)m_ucScale * (1/64.0f)` -- corroborated by the chunk
    // loader (scale*64 clamp @0x1803FC834), CalcMatrix (0x18065F1D0) and GetMaxViewDist.
    virtual float GetScale() = 0;
};
static_assert(sizeof(IVegetation) == 0x50, "IVegetation adds no data over IRenderNode");

}  // namespace Offsets
