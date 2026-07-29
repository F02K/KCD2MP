#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_LODHandle -- LOD handle interface (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4F6E608.  NO standalone COL/vtable -- appears only as a
// BaseClassDescriptor (rva 0x4567F38) in C_LODAgentNPC's base array (mdisp 0,
// primary subobject).  The realized primary vtable (C_LODAgentNPC, rva
// 0x3A83778, 9 slots) is SHARED between I_LODHandle, a co-located
// std::_Func_base<void, C_NPC*>, and C_LODAgentNPC's own virtuals -- the
// per-interface slot split needs per-slot decompiles and is UNRESOLVED [U].
// Modeled minimally as a vptr-only interface with a dtor placeholder [U].

namespace wh::xgenaimodule {

class I_LODHandle {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_LODHandle;
    virtual ~I_LODHandle();   // [U placement -- slot split unresolved]
};
static_assert(sizeof(I_LODHandle) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
