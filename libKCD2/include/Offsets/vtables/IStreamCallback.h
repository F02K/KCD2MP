#pragma once
#include <memory>

// -----------------------------------------------
// IStreamCallback -- CrySystem async-read completion interface, KCD2 binary slot order
// (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// class (RTTI .?AVIStreamCallback@@). Standalone default vtable 0x183A39AC8 = exactly 4 slots
// (dtor 0x182417790 + 3x _purecall). Implemented as a secondary base by CStatObj (+0x30,
// vtable 0x183A2F760), CMergedMeshRenderNode (+0x58, 0x18402A758) and COctreeNode (+0x18).
//
// [WH ABI DEVIATION] The callbacks do NOT take stock CE3.8's (IReadStream*, unsigned nError).
// Each real impl receives ONE argument shaped { IReadStream* ptr; _Ref_count_base* ctrl } --
// an MSVC std::shared_ptr<IReadStream> BY VALUE (callee releases the control block with the
// canonical two-stage decrement, e.g. 0x180767c2c). Slots [2]/[3] are pinned by their own log
// strings in both implementors ("...StreamAsyncOnComplete..." / "...StreamOnComplete...");
// slot [1] is the remaining callback (StreamOnNeedStorage shape, returns a pointer) --
// identity UNVERIFIED, stock declaration order does not hold here.
// Evidence: mesh_engine_re/mergedmesh.md §4, cstatobj.md §4.
// Observed IReadStream vfuncs: +0x00 is-error, +0x10 buffer, +0x18 bytes read, +0x68 error
// code, +0x70 error name, +0x80 free temp memory.

struct IReadStream;  // CrySystem type, not yet RE'd

namespace Offsets {

class IStreamCallback {
public:
    virtual ~IStreamCallback() = default;                                       // [0] impls are adjustor thunks into the owner's deleting dtor
    virtual void* _vf1(std::shared_ptr<IReadStream> pStream) = 0;               // [1] default-ish no-op: releases the payload, returns nullptr (StreamOnNeedStorage hypothesis)
    virtual void StreamAsyncOnComplete(std::shared_ptr<IReadStream> pStream) = 0;  // [2] worker-thread completion (string-pinned in CStatObj 0x1805229DC + CMergedMeshRenderNode 0x180767A48)
    virtual void StreamOnComplete(std::shared_ptr<IReadStream> pStream) = 0;    // [3] main-thread completion (string-pinned: 0x18105DFBC / 0x18043A9B8)
};
static_assert(sizeof(IStreamCallback) == 0x8, "vptr-only interface");

}  // namespace Offsets
