#pragma once
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PhaseItemEventImplementationBase :
// I_AnimEventImplementation -- ABSTRACT intermediate factoring shared bodies for
// the two item-phase events (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4FF2DE0.  NO COL / NO standalone vtable (VERIFIED full-.rdata
// scan) -- ctor inlined into the derived ctors; do NOT emit a vtable Offsets
// entry.  Adds NO new virtual slots (both subclasses stay at 13); factors the
// shared [4] = sub_180C85240 default for the phase pair.

namespace wh::xgenaimodule {

class C_PhaseItemEventImplementationBase : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PhaseItemEventImplementationBase;
    void* BuildStringA(void* out) override;   // [4] sub_180C85240 (shared by both subclasses)
};
static_assert(sizeof(C_PhaseItemEventImplementationBase) == 0x08, "vptr only");

}  // namespace wh::xgenaimodule
