#pragma once

// -----------------------------------------------
// IRenderNodeInstanceStreamingCallback -- Cry3DEngine render-node instance-streaming callback
// (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Primary base of wh::environmentmodule::C_StreamingHelper, which registers itself with the
// 3D engine (qword_18492D808 vfunc +2056). Vptr-only layout stub: the virtual set is not
// enumerated; only the vptr matters for the host layout. Do NOT call through this stub.

namespace Offsets {

struct IRenderNodeInstanceStreamingCallback {
    virtual void _vf0() = 0;   // slots not enumerated
};
static_assert(sizeof(IRenderNodeInstanceStreamingCallback) == 0x08, "vptr-only interface");

}  // namespace Offsets
