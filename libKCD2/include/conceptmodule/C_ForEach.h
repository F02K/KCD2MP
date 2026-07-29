#pragma once
#include <cstdint>
#include "C_EventNode.h"
#include "C_PortRef.h"
#include "C_TemplatedNode.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"

// -----------------------------------------------
// wh::conceptmodule::C_ForEach -- array iteration loop
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x238, vtable 0x183E2B3B8
// (43 slots).  343 corpus uses.
// -----------------------------------------------
// Chain: C_TemplatedNode -> C_EventNode<C_TemplatedNode> (vt 0x183E2B518) -> here.
// Ctor 0x1806B35C0, creator 0x1811E3D50; rttr "wh::conceptmodule::ForEach"
// (reg sub_1810261F4; Value/Index carry LinkedTriggerPorts = ["OnValue"]).
// OnExecute [33] 0x180619BDC: Begin fired -> RunLoop 0x180619F8C (re-entrancy
// guarded by m_isRunning): read Array, require is_sequential_container
// (type_data flags & 0x200), then per element {Value, Index} into the event map and
// EmitEvent(OnValue) [42] 0x180619E14 -- SYNCHRONOUS per element; honor
// m_breakRequested; finally fire OnEnd (0x180619B1C).  Break fired while running ->
// set m_breakRequested.

namespace wh::conceptmodule {

class C_ForEach : public C_EventNode<C_TemplatedNode> {
public:
    RTTR_ENABLE(C_EventNode<C_TemplatedNode>)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x18127FDF0 event-map lookup
    void EnumerateNodeVariants() override;      // [27] 0x18268E474 "Iterates over an array of elements"
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x180619BDC Begin -> RunLoop; Break -> flag

    void RunLoop();                             // 0x180619F8C (the iteration body)

    C_TypedPortRef<S_Trigger> m_begin;    // +0x70  "Begin"   In trigger
    C_TypedPortRef<S_Trigger> m_break;    // +0xB0  "Break"   In trigger
    C_PortRef                 m_array;    // +0xF0  "Array"   In (Template = TypeT)
    C_TypedPortRef<S_Trigger> m_onValue;  // +0x130 "OnValue" Out trigger (per element)
    C_TypedPortRef<S_Trigger> m_onEnd;    // +0x170 "OnEnd"   Out trigger
    C_PortRef                 m_value;    // +0x1B0 "Value"   Out (Template = TypeT_0, event-scoped)
    C_TypedPortRef<unsigned int> m_index; // +0x1F0 "Index"   Out (event-scoped)
    bool m_isRunning;                     // +0x230 re-entrancy guard
    bool m_breakRequested;                // +0x231
    uint8_t _pad232[6];                   // +0x232
};
static_assert(sizeof(C_ForEach) == 0x238, "C_ForEach must be 0x238");

}  // namespace wh::conceptmodule
