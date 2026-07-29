#pragma once
#include "I_SmartEntityListener.h"

// -----------------------------------------------
// wh::xgenaimodule::I_SmartAreaListener -- smart-area event listener (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SmartAreaListener@xgenaimodule@wh@@.  In C_SmartAreaSubBrain
// BOTH I_SmartAreaListener and I_SmartEntityListener sit at the SAME mdisp
// (+0xD0) with ONE combined 12-slot subobject vtable (rva 0x3FD7FB8).  MODELED
// as I_SmartAreaListener : I_SmartEntityListener adding slots [10][11] -- the
// derivation (vs vtable folding of two unrelated ifaces) is the natural reading
// of the shared mdisp but is UNVERIFIED [U].

namespace wh::xgenaimodule {

class I_SmartAreaListener : public I_SmartEntityListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SmartAreaListener;
    virtual void SalVf10() = 0;   // [10] [U role]
    virtual void SalVf11() = 0;   // [11] [U role]
};
static_assert(sizeof(I_SmartAreaListener) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
