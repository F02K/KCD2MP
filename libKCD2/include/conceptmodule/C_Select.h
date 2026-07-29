#pragma once
#include "C_EventNode.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"
#include "rttr/type.h"

// -----------------------------------------------
// wh::conceptmodule::C_Select -- N-way event multiplexer
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xE0, vtable 0x183B56D60 (42 slots).
// 191 corpus uses.
// -----------------------------------------------
// Ctor 0x1812EEB24, creator 0x1812EEA6C; rttr type "wh::conceptmodule::Select"
// (reg sub_18115B51C).  Pin grammar (GetPortDefinitions [28] 0x1804EFE64): group
// i = 0..InputPortGroups-1 gets In trigger "Select<'A'+i>" plus In data pins
// "<'A'+i>Value<N>", N = 1..len(ValueTypes); single Out trigger "OnExec".
// OnExecute [33] 0x181939800 takes the LAST CHARACTER of the fired port's name as
// the group selector, gathers that group's values into an event-data map and
// EmitEvents through OnExec -- downstream reads the values only while the trigger
// propagates (C_EventNode semantics).

namespace wh::conceptmodule {

class C_Select : public C_EventNode<C_Node> {
public:
    RTTR_ENABLE(C_EventNode<C_Node>)   // [5..7] trio overrides
    void EnumerateNodeVariants() override;  // [27]
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x1804EFE64 group grammar
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x181939800 gather group -> EmitEvent(OnExec)

    C_TypedPortRef<S_Trigger> m_onExec;  // +0x68  rttr "OnExec" Out trigger
    int32_t m_inputPortGroups;           // +0xA8  rttr "InputPortGroups" -- number of Select groups
    uint8_t _padAC[4];                   // +0xAC
    std::vector<uint64_t> m_unkB0;       // +0xB0  ctor-zeroed, purpose unknown [U]
    std::vector<rttr::type> m_valueTypes;  // +0xC8  rttr "ValueTypes" -- one value pin per entry per group
};
static_assert(sizeof(C_Select) == 0xE0, "C_Select must be 0xE0");

}  // namespace wh::conceptmodule
