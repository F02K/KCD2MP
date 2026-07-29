#pragma once
#include <cstdint>
#include <functional>

// -----------------------------------------------
// IMergedMeshRenderNode -- WH instance-enumeration surface of merged-mesh sector nodes,
// KCD2 binary slot order (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// struct (RTTI .?AUIMergedMeshRenderNode@@), secondary base of CMergedMeshRenderNode at +0x50;
// subobject vtable 0x18402A780 = exactly 3 slots (next COL pSelf-verified). Pure abstract --
// no standalone default vtable exists. This surface has NO stock-GameSDK equivalent: it is the
// WH-added hook the harvest/respawn system uses to enumerate and hide/restore individual
// vegetation instances (hide = zero the instance's scale byte, restore = copy back the WH
// scaleOriginal backup at instance+0xE).
//
// All three impls require node state == 5 (STREAMED_IN). The std::function parameters are
// taken BY VALUE and DESTROYED by the callee (MSVC callee-destroy ABI: caller builds a temp,
// passes its address). Callback argument shapes are partially UNVERIFIED (the transform blob
// is 6 dwords, decoder sub_18043C4D0 not analysed) -- treat the signatures below as layout-
// faithful but semantically provisional. Evidence: mesh_engine_re/mergedmesh.md §3.

namespace Offsets {

struct IMergedMeshRenderNode {
    // [0] 0x18043C258 -- enumerate instances whose group has StatInstGroup+0x10A set; where the
    // predicate returns true, write instance.scale(+0xC) = bVisible ? instance.scaleOriginal(+0xE) : 0.
    // Predicate args: (const void* xform6dw, float scale, const CryStringT<char>* meshName, const uint32_t* nameCrcLower).
    virtual bool SetInstancesVisible(std::function<bool(const void*, float, const CryStringT<char>*, const uint32_t*)> filter,
                                     bool bVisible) = 0;
    // [1] 0x18043BFEC -- read-only enumeration, same walk and callback shape (no scale write).
    virtual bool ForEachInstance(std::function<void(const void*, float, const CryStringT<char>*, const uint32_t*)> fn) = 0;
    // [2] 0x18043B52C -- `return node->State(+0x6C) == 5;`
    virtual bool IsStreamedIn() = 0;
};
static_assert(sizeof(IMergedMeshRenderNode) == 0x8, "vptr-only interface");

}  // namespace Offsets
