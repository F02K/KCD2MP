#pragma once
#include "C_InputTriggerPort.h"

// -----------------------------------------------
// wh::conceptmodule::C_AutoTriggerPort -- wrapper that auto-fires another port
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x50, vtable 0x183A46168 (31 slots).
// -----------------------------------------------
// NOT created by GetOrCreatePort: only MaybeMakeAutoTrigger 0x181E27A00 wraps an
// ALREADY-EXISTING port whose definition passes the gates (definition present;
// direction/def-vslot-4 filter; def IsAutoTriggerable [vslot 15]; port vcall+0x60
// arg 3).  Ctor 0x1806B1C84(_smart_ptr<I_Port> wrapped): takes the SAME name as the
// wrapped port (I_Port named ctor 0x1806B2958) and a weak handle to it; appended to
// the auto-trigger owner list at (owner+8)+0x38 (0x181E28850).  Inherits the sink
// Trigger [15] 0x1806920D4 -> firing it runs the owner node directly.

namespace wh::conceptmodule {

class C_AutoTriggerPort : public C_InputTriggerPort {
public:
    RTTR_ENABLE(C_InputTriggerPort)   // [5] 0x1806B1BCC, [7] 0x18262CEC4

    S_ResourceRef m_wrapped;          // +0x30  weak handle to the wrapped port (0x180693A70)
    std::vector<S_ResourceRef> m_unk38;  // +0x38  zero-init, never written in any examined path [U]
};
static_assert(sizeof(C_AutoTriggerPort) == 0x50, "C_AutoTriggerPort must be 0x50");

}  // namespace wh::conceptmodule
