#pragma once
#include "C_Node.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"

// -----------------------------------------------
// wh::conceptmodule::C_TriggerSequence -- ordered trigger fan-out
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xC0, vtable 0x183B1CFB0
// (41 slots).  2,093 corpus uses.
// -----------------------------------------------
// Direct C_Node child (no TypeT; the first port at +0x40 -- not +0x48 -- is direct
// proof that C_TemplatedNode's extra 8 bytes are m_typeT).  Ctor 0x18115D240,
// creator 0x18115D188; rttr "wh::conceptmodule::TriggerSequence"
// (reg sub_1810DED70).  Editor text (authoritative): "Triggers output ports in a
// deterministic order from the first to the last" -- the authored Out* pins are
// runtime ports (same mechanism as MakeArray's A..X) fired in declaration order by
// OnExecute [33] 0x1818AAB9C [loop body via sub_181E2CAF0, UNVERIFIED].

namespace wh::conceptmodule {

class C_TriggerSequence : public C_Node {
public:
    RTTR_ENABLE(C_Node)   // [5..7] trio overrides
    void EnumerateNodeVariants() override;  // [27] 0x18268ECD8
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x1818AAB9C fire outputs first -> last

    C_TypedPortRef<S_Trigger> m_exec;          // +0x40  "Exec" In trigger
    C_TypedPortRef<S_Trigger> m_outPrototype;  // +0x80  variadic Out prototype (real outputs are runtime ports)
};
static_assert(sizeof(C_TriggerSequence) == 0xC0, "C_TriggerSequence must be 0xC0");

}  // namespace wh::conceptmodule
