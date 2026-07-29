#pragma once
#include "I_Port.h"

// -----------------------------------------------
// wh::conceptmodule::C_DebuggerPort -- editor/debug tap port
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x30, vtable 0x183E250C0 (23 slots).
// -----------------------------------------------
// I_Port-shaped (no C_EdgePort layer, no members).  rttr creators sub_18264C074 /
// sub_18264C1E4 alloc 0x30.  Role stays None -> never an edge endpoint; used by the
// Skald debugger to observe values/triggers without wiring into the graph.

namespace wh::conceptmodule {

class C_DebuggerPort : public I_Port {
public:
    RTTR_ENABLE(I_Port)                     // [5] 0x181A6D2D4, [7] 0x1826534FC
    bool IsPassThrough() const override;    // [10] 0x182650894
    void Trigger() override;                // [15] 0x1826509AC
    rttr::variant GetValue() override;      // [16] 0x182650908
};
static_assert(sizeof(C_DebuggerPort) == 0x30, "C_DebuggerPort adds nothing over I_Port");

}  // namespace wh::conceptmodule
