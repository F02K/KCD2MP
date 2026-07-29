#pragma once
#include "C_AutoTriggerable.h"
#include "C_EventNode.h"
#include "C_PortRef.h"
#include "C_TemplatedNode.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"

// -----------------------------------------------
// wh::conceptmodule::C_StateWatch -- the Skald "Watch" node
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x130, vtable 0x183E299E8
// (45 slots).  rttr name "wh::conceptmodule::Watch" (reg sub_180F6200C).
// -----------------------------------------------
// Chain: C_TemplatedNode (+[41] OnTypeTSet) -> C_AutoTriggerable<C_TemplatedNode>
// (+[42] ResolveTriggerSource 0x1806B1478, [43]) -> C_EventNode<...> (+[44]
// EmitEvent 0x180619E14) -> here; 45 slots total confirms the arithmetic.  Ctor
// 0x180ACEC1C, creator 0x180ACEB64.  Editor text: "Node which notifies about data
// changed in watched state. There is no guarantee it will trigger only when the
// state was really changed - it can trigger more often."  Auto-triggerable
// observer: fires OnChanged (with the value event-scoped) when the WatchedState
// pin's source changes, gated by IsActive.  OnExecute [33] 0x180DC3E10;
// GetPortValue [12] 0x1826B9BC8 (event map); GetPortDefinitions [28] 0x180ACFCD0.
// Port offsets forced by sizeof: base 0x70 + 3 x 0x40 = 0x130.

namespace wh::conceptmodule {

class C_StateWatch : public C_EventNode<C_AutoTriggerable<C_TemplatedNode>> {
public:
    RTTR_ENABLE(C_EventNode<C_AutoTriggerable<C_TemplatedNode>>)   // [5..7] trio overrides
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] 0x1826B9BC8 event-map lookup
    void EnumerateNodeVariants() override;  // [27] 0x18268E9EC
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x180ACFCD0
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x180DC3E10 watched-state change -> EmitEvent(OnChanged)

    C_PortRef                 m_watchedState;  // +0x70  rttr "WatchedState" -- In (Template = TypeT)
    C_TypedPortRef<bool>      m_isActive;      // +0xB0  rttr "IsActive" -- In gate
    C_TypedPortRef<S_Trigger> m_onChanged;     // +0xF0  rttr "OnChanged" -- Out trigger (value event-scoped)
};
static_assert(sizeof(C_StateWatch) == 0x130, "C_StateWatch must be 0x130");

}  // namespace wh::conceptmodule
