#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_Intellect -- root abstract interface of the AI BRAIN family
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vptr only; C_AIBrain's first own
// member is at +0x08, RTTI numContained=0).
// -----------------------------------------------
// RTTI TD rva 0x4EFD7F0.  NO standalone COL/vtable anywhere in the image (never
// most-derived); its vtable is the leading portion of C_AIBrain's 35-slot table.
// SLOT-SPLIT UNVERIFIED [U]: which of the 35 slots are the I_Intellect contract
// vs C_AIBrain's own new virtuals cannot be recovered (C_AIBrain is the only
// implementer).  Modeled minimally: only the [0] dtor slot is declared here; the
// remaining 34 slots are declared on C_AIBrain (layout-identical either way).
// Best hypothesis (NOT modeled): the 15 pure slots are the I_Intellect contract.

namespace wh::xgenaimodule {

class I_Intellect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Intellect;
    virtual ~I_Intellect();   // [0] (C_AIBrain impl: scalar deleting dtor sub_183208590)
};
static_assert(sizeof(I_Intellect) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
