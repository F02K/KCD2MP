#pragma once
#include "I_AnimEventImplementation.h"

// -----------------------------------------------
// wh::xgenaimodule::C_MonologEventImplementation : I_AnimEventImplementation --
// ABSTRACT intermediate for the two monolog events; introduces vf13
// GetFormatString (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4FF2CF0.  NO COL / NO standalone vtable (VERIFIED) -- ctor
// inlined into derived ctors; do NOT emit a vtable Offsets entry.  Both
// subclasses have 14 slots -> vf13 is introduced HERE.  Whether the base decl is
// pure or has a hidden default is UNVERIFIED -- modeled as a plain virtual [U].

namespace wh::xgenaimodule {

class C_MonologEventImplementation : public I_AnimEventImplementation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MonologEventImplementation;
    virtual const char* GetFormatString();   // [13] NEW slot (both subclasses override) [U pure-vs-default]
};
static_assert(sizeof(C_MonologEventImplementation) == 0x08, "vptr only");

}  // namespace wh::xgenaimodule
