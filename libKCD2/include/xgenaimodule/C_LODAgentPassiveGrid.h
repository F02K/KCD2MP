#pragma once
#include "C_LODAgent.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LODAgentPassiveGrid : C_LODAgent -- abstract intermediate
// for LOD agents whose activation adds/removes their owner to/from a passive
// spatial grid (KCD2 WHGame.dll 1.5.6, kd7u).  Abstract, fold-only: NO own
// COL/vtable materializes in the binary (byte-search of its TD rva finds no
// standalone COL); it exists only inside derived classes.  No independently
// provable size -> no static_assert.
// -----------------------------------------------
// RTTI TD rva 0x4EFF3F8 (proven as CHD base #1 of C_LODAgentPerceptibleVolume,
// mdisp 0).  The grid-membership state (cell handle / owner backptr, layered at
// +0x30/+0x38 in the derived class) conceptually belongs to this layer, but the
// exact PassiveGrid-vs-derived field split is [UNVERIFIED] (no PassiveGrid ctor
// exists) -- the fields are therefore declared in the derived class.

namespace wh::xgenaimodule {

class C_LODAgentPassiveGrid : public C_LODAgent {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LODAgentPassiveGrid;
};

}  // namespace wh::xgenaimodule
