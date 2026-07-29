#pragma once
#include "../../cry3dengine/CLodValue.h"
#include "../../cry3dengine/EERType.h"

// -----------------------------------------------
// IShadowCaster -- Cry3DEngine shadow-caster interface, KCD2 binary slot order
// (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// Primary (mdisp 0) base of IRenderNode; owns slots [0..7] of every render-node vtable.
// Slot count CERTIFIED via its standalone default vtable 0x183A33A70: exactly 8 slots, ending
// at the COL of IRenderNode (0x183A33AB0). Contributes NO data -- its dtor (0x18348E81C) only
// restores the vptr, and IRenderNode's members start at +0x08 right after the shared vptr.
// Interfuscator shuffles SDK interface vtables: this order is authoritative for THIS binary
// only; do not cross-port slot indices. Evidence: analysis/mesh_engine_re/irendernode.md.
//
// Default impls in 0x183A33A70: [1] retfalse, [2] 0x180659750 (CLodValue(wantedLod) ctor);
// [3][4][5][6][7] _purecall (IRenderNode later defaults [5] and [6]).

struct SRendParams;         // SDK type, not RE'd -- passed by const& only
struct SRenderingPassInfo;  // SDK type, not RE'd -- passed by const& only

namespace Offsets {

struct IShadowCaster {
    virtual ~IShadowCaster() = default;                                            // [0] deleting dtor in impls
    virtual bool _vf1() = 0;                                                       // [1] returns false in ALL impls (0x180838AE0); no behavioural signal -- do not name
    virtual CLodValue ComputeLod(int wantedLod, const SRenderingPassInfo& passInfo) = 0;  // [2] default 0x180659750 = CLodValue(wantedLod)
    virtual void Render(const SRendParams& rParams, const SRenderingPassInfo& passInfo) = 0;  // [3] pure; CDecal impl reads SRendParams+0x64 fDistance
    virtual AABB GetBBoxVirtual() = 0;                                             // [4] AABB BY VALUE (sret, impls return the sret ptr in rax) -- do NOT overload with FillBBox
    virtual void FillBBox(AABB& aabb) = 0;                                         // [5] void + out-param (no rax); IRenderNode default 0x1834914F0 = `aabb = GetBBoxVirtual()`
    virtual void* _vf6() = 0;                                                      // [6] returns nullptr in all audited impls (0x18066CD10); do not name
    virtual EERType GetRenderNodeType() = 0;                                       // [7] per-class constant stub; see EERType.h
};
static_assert(sizeof(IShadowCaster) == 0x8, "vptr-only interface");

}  // namespace Offsets
