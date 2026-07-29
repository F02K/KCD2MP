#pragma once

// -----------------------------------------------
// IMergeMeshStreamListener -- Cry3DEngine merged-mesh streaming callback (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Second base of wh::environmentmodule::C_RespawnManager (RTTI mdisp 0x08). Vptr-only layout
// stub: the virtual set is not enumerated (interfuscator-shuffled SDK side); only the vptr
// matters for the host layout. Do NOT call through this stub.

namespace Offsets {

struct IMergeMeshStreamListener {
    virtual void _vf0() = 0;   // slots not enumerated
};
static_assert(sizeof(IMergeMeshStreamListener) == 0x08, "vptr-only interface");

}  // namespace Offsets
